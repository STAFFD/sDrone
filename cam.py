from cv2 import *

cam = VideoCapture(1)

while True:
	if cam.isOpened():
		ret, frame = cam.read()
		if frame.size > 0:
			imshow("flight", frame)
			waitKey(1)