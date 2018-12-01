# expression-recognition
facial expression recognition

# Environment
tflearn0.3.2 
Qt5.7 
Python3.5

# Funciton
We used AlexNet architecture to trainging a facial expression recognition model based on RAF databases(http://www.whdeng.cn/RAF/model1.html).  

The trained model is at https://pan.baidu.com/s/1873EPi3qHe_VvDWyV0Mk2w  password：l2mu 

RAF database includes "Anger", "Disgust", "Fear", "Happy", "Neutral", "Sad", "Surprise" seven expression tags. 

Client based on Qt5.7, which captured a image from camera, send it to server, and get expression tag. 

Server based on Python3.5, whick receive a image from client, return expression tag. 

# Result
![image](https://github.com/XPping/expression-recognition/raw/master/python_server/result.png)
