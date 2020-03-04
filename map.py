import serial
# import webview


# webview.create_window('Hello world', 'https://m.amap.com/navi/?dest=116.470098,39.992838&destName=haha&hideRouteIcon=1&key=5a2f3416e6db2c1f133073c7c5f75ac2')

# webview.start()

serialPort = "/dev/cu.usbserial-14140"

baudRate = 9600

ser = serial.Serial(serialPort, baudRate, timeout=0.5)




while True:
	data = str(ser.readline().decode("utf-8")).split(" ")


	print(data)

ser.close()