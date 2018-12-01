from __future__ import division, print_function, absolute_import
import os
# os.environ["CUDA_VISIBLE_DEVICES"] = "0"
import tflearn
from tflearn.layers.core import input_data, dropout, fully_connected
from tflearn.layers.conv import conv_2d, max_pool_2d
from tflearn.layers.normalization import local_response_normalization
from tflearn.layers.estimator import regression
import cv2
import numpy as np

#Opencv face detect
def faceDetector(image):
    frame = image
    classfier = cv2.CascadeClassifier("haarcascade_frontalface_alt.xml")
    divisor = 8
    h,w = frame.shape[0:2]
    min_size = (w//divisor, h//divisor)
    face_rects = classfier.detectMultiScale(frame, 1.2, 2, cv2.CASCADE_SCALE_IMAGE, min_size)

    if len(face_rects) == 0:
        return None
    x,y,w,h = face_rects[0]
    cv2.rectangle(frame,(x,y),(x+w,y+h),(0,255,0),3)
    return frame, [x, y, x+w, y+h]
def loadOneImage(path):
    image = cv2.imread(path)
    face = faceDetector(image)
    if face is None:
        return None
    face, rect = face 
    face = cv2.resize(face, (227, 227))

    faces = np.array([face], dtype=np.float32) / 255.0
    return faces, rect

# trainX, trainY = X[0:int(len(X)*0.9)], Y[0:int(len(X)*0.9)]
# valX, valY = X[int(len(X)*0.9)::], Y[int(len(X)*0.9)::]

# Building 'AlexNet'
def buildNetwork():
    network = input_data(shape=[None, 227, 227, 3])
    network = conv_2d(network, 96, 11, strides=4, activation='relu')
    network = max_pool_2d(network, 3, strides=2)
    network = local_response_normalization(network)
    network = conv_2d(network, 256, 5, activation='relu')
    network = max_pool_2d(network, 3, strides=2)
    network = local_response_normalization(network)
    network = conv_2d(network, 384, 3, activation='relu')
    network = conv_2d(network, 384, 3, activation='relu')
    network = conv_2d(network, 256, 3, activation='relu')
    network = max_pool_2d(network, 3, strides=2)
    network = local_response_normalization(network)
    network = fully_connected(network, 4096, activation='tanh')
    network = dropout(network, 0.5)
    network = fully_connected(network, 4096, activation='tanh')
    network = dropout(network, 0.5)
    network = fully_connected(network, 7, activation='softmax')
    network = regression(network, optimizer='momentum',
                         loss='categorical_crossentropy',
                         learning_rate=0.001)

    # Training
    model = tflearn.DNN(network, checkpoint_path='model_alexnet',
                        max_checkpoints=1, tensorboard_verbose=2)
    #model.fit(X, Y, n_epoch=100, validation_set=0.1, shuffle=True,
    #          show_metric=True, batch_size=32, snapshot_step=200,
    #          snapshot_epoch=False, run_id='alexnet_oxflowers17')
    #model.save("models.h5")
    model.load(r"model/models.h5")
    return model

model = buildNetwork()

def predict(path):
    global model
    data = loadOneImage(path)
    if data is None:
        return None
    data, rect = data 
    return model.predict(data)[0].tolist(), rect
"""
label, rect = predict("2.jpg")
print(label, type(label), len(label), rect)
sendData = ""
for i in range(len(label)):
    print(label[i])
    sendData = sendData + str(float(label[i]))+","
sendData = sendData[0:-1]
print(sendData)
"""