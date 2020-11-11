import tensorflow as tf
from tensorflow.keras import Model, layers

VF_COEF = 0.01
EPSILON_CLIP = 0.2
ENTROPY_COEF = 0.001
eps = np.finfo(np.float32).eps.item()

def ppo_loss_discrete(advantage, old_pred, y_true, y_pred, val_true, val_pred):
    """PPO discreate loss function.
    Args:
        advantage: advantage from the episode.
        old_pred: old softmax predictions.
        y_true: actions taken in the episode.
        y_pred: new softmax predictions.
        val_true: true values (i.e., returns) from the episode.
        val_pred: critic value predictions
    
    Returns:
        A tuple of PPO loss and value MSE loss 
    """
      
    new_prob = tf.math.log(tf.reduce_sum(y_true * y_pred, axis=-1) + eps)
    old_prob = tf.math.log(tf.reduce_sum(y_true * old_pred, axis=-1) + eps)
    old_prob = tf.cast(old_prob, tf.float32)
    ratio = tf.exp(new_prob - old_prob)
    surrogate_1 = ratio * advantage
    surrogate_2 = tf.clip_by_value(ratio, 1.0 - EPSILON_CLIP, 1.0 + EPSILON_CLIP) * advantage
    vf_loss = VF_COEF * tf.reduce_mean(tf.square(val_true - val_pred))  
    entropy_pen = -ENTROPY_COEF * tf.reduce_sum(y_pred * tf.math.log(y_pred + 1e-10), axis=1)
    policy_loss = -tf.reduce_mean(tf.minimum(surrogate_1, surrogate_2) + entropy_pen)
    #policy_loss = -tf.reduce_mean(new_prob * advantage)
    return  policy_loss, vf_loss

class ActorCritic(Model):
    def __init__(self, args, num_actions=1024):
        self._args = args
        super().__init__()
        
        # Embedding for the type of object
        self._type_embedding = layers.Embedding(self._args.num_types, self._args.type_emb)

        # Used for concatenating the type embedding to the rest of the object
        self._concat = layers.Concatenate()

        # Embedding for the object
        self._obj_transform_emb = layers.Conv1D(self._args.obj_emb, kernel_size=1, strides=1, activation="relu") 

        self._flatten = layers.Flatten()
        self._dropout = layers.Dropout(self._args.dropout_rate)
        
        self._lstm_1 = layers.LSTM(self._args.lstm_units,
                stateful=True,
                return_sequences=True)

        self._lstm_2 = layers.LSTM(self._args.lstm_units,
                stateful=True)
        
        self._actor_dense = layers.Dense(self._args.dense_units, activation="relu")
        self._critic_dense= layers.Dense(self._args.dense_units, activation="relu")

        self._actor_out = layers.Dense(num_actions, activation="softmax")
        self._critic_out = layers.Dense(1)

    def call(self, x, training=True):
        # Get the object types
        obj_types = x[:, :, -1]

        # Embed the type
        type_embs = self._type_embedding(obj_types)

        # Add to the type emb to object representation
        obj_reps = self._concat([tf.cast(x[:, :, :-1], tf.float32), type_embs])
        
        # Create the object embeddings
        obj_reps = self._obj_transform_emb(obj_reps)
        obj_reps = self._flatten(obj_reps)
        obj_reps = self._dropout(obj_reps, training=training)

        # Run through stacked LSTM
        obj_reps = obj_reps[:, tf.newaxis, :]
        obj_reps = self._lstm_1(obj_reps)
        obj_reps = self._lstm_2(obj_reps)

        # Actor/Critic independent layers
        actor_dense_output = self._actor_dense(obj_reps)
        critic_dense_output = self._critic_dense(obj_reps)
        actor_output = self._actor_out(actor_dense_output)
        critic_output = self._critic_out(critic_dense_output)

        return actor_output, critic_output