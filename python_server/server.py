#!/usr/bin/env python
# -*- coding: utf-8 -*-

import socket
import sys
import threading
import os
import cv2
import alexnet
def server():
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        s.bind(("127.0.0.1", 6666))
        s.listen(10)
        print("Listening...")
    except socket.error as msg:
        print(msg, "error")
        sys.exit(1)
    while True:
        conn, addr = s.accept()
        t = threading.Thread(target=dealClientData, args=(conn, addr))
        t.start()
    s.close()

# load alexnet to predict a image 
def sendResult(conn, image_path):
    label = alexnet.predict(image_path)
    if label is None:
        sendData = "0,0,0,0,0,0,0,0,0,0,0"
    else:
        label, rect = label 
        sendData = ""
        for r in rect: sendData = sendData + str(r) + ","
        for l in label:
            sendData = sendData + str(float(l))+","
        sendData = sendData[0:-1]
    # sendData = "0.08,0.12,0.1,0.2,0.05,0.05,0.4"
    send = bytes(sendData, encoding = "utf8")
    conn.sendall(send)

# Receive data from client
count = 0
def dealClientData(conn, addr):
    global count
    End = b"Send End"
    print(addr)
    count += 1
    image_path = "{}.jpg".format(count%3)
    imageFile = open(image_path, "wb")
    recvData = b""
    while True:
        data = conn.recv(1024*1024)
        recvData += data
        if End in recvData:
            recvData = recvData[0:recvData.find(End)]
            break 
    print(len(recvData))
    imageFile.write(recvData)
    sendResult(conn, image_path)
    conn.close()
server()
