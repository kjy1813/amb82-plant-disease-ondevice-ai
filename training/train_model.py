# CNN training script for the AMB82-mini plant disease classifier
# Course: Embedded Systems Experiment (team project, 2025)
#
# Produces model.h5, which is then converted to an AmebaPro2 .nb file
# (see ../model/model_conversion.md).
#
# Portfolio cleanup (2026): DATA_DIR was changed from a machine-specific
# absolute path to a relative 'dataset' folder. Nothing else was changed.

import os
import tensorflow as tf
from tensorflow.keras import models, layers
from tensorflow.keras.preprocessing.image import ImageDataGenerator
from tensorflow.keras.layers import Conv2D, BatchNormalization, MaxPooling2D, Flatten, Dense, Dropout

IMG_WIDTH, IMG_HEIGHT = 224, 224
BATCH_SIZE = 16
EPOCHS = 30
NUM_CLASSES = 3 # set to the number of class folders under DATA_DIR (2 for the cabbage model)

DATA_DIR = 'dataset'

train_datagen = ImageDataGenerator(
    rescale=1./255,
    validation_split=0.1,
    horizontal_flip=True,
    vertical_flip=True,
    shear_range=0.2,
    zoom_range=0.2,
    width_shift_range=0.1,
    height_shift_range=0.1
)

test_datagen = ImageDataGenerator(
    rescale=1./255,
    validation_split=0.1
)

train_generator = train_datagen.flow_from_directory(
    DATA_DIR,
    target_size=(IMG_WIDTH, IMG_HEIGHT),
    batch_size=BATCH_SIZE,
    class_mode='categorical',
    subset='training'
)

test_generator = test_datagen.flow_from_directory(
    DATA_DIR,
    target_size=(IMG_WIDTH, IMG_HEIGHT),
    batch_size=BATCH_SIZE,
    class_mode='categorical',
    subset='validation'
)

def create_model():
    model = models.Sequential()

    model.add(Conv2D(16, (3,3), padding='same', input_shape=(IMG_WIDTH, IMG_HEIGHT, 3), activation='relu'))
    model.add(MaxPooling2D(pool_size=(2,2)))
    model.add(BatchNormalization())

    model.add(Conv2D(32, (3,3), padding='same', activation='relu'))
    model.add(MaxPooling2D(pool_size=(2,2)))
    model.add(BatchNormalization())

    model.add(Conv2D(64, (3,3), padding='same', activation='relu'))
    model.add(MaxPooling2D(pool_size=(2,2)))
    model.add(BatchNormalization())

    model.add(Conv2D(128, (3,3), padding='same', activation='relu'))
    model.add(MaxPooling2D(pool_size=(2,2)))
    model.add(BatchNormalization())

    model.add(Conv2D(256, (3,3), padding='same', activation='relu'))
    model.add(MaxPooling2D(pool_size=(2,2)))
    model.add(BatchNormalization())

    model.add(Conv2D(512, (3,3), padding='same', activation='relu'))
    model.add(MaxPooling2D(pool_size=(2,2)))
    model.add(BatchNormalization())

    model.add(Flatten())
    model.add(Dense(1024, activation='relu'))
    model.add(Dense(64, activation='relu'))
    model.add(Dense(NUM_CLASSES, activation='softmax'))

    return model

model = create_model()
model.summary()

model.compile(loss='categorical_crossentropy', optimizer='adam', metrics=['accuracy'])

print("Starting training...")
history = model.fit(
    train_generator,
    validation_data=test_generator,
    epochs=EPOCHS
)

print("Saving model to model.h5...")
model.save('model.h5', include_optimizer=False)
print("Model saved.")

print("Evaluating model...")
test_loss, test_acc = model.evaluate(test_generator)
print(f"Test Loss: {test_loss}")
print(f"Test Accuracy: {test_acc}")
