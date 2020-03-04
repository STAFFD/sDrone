import serial
# import webview


# webview.create_window('Hello world', 'https://m.amap.com/navi/?dest=116.470098,39.992838&destName=haha&hideRouteIcon=1&key=5a2f3416e6db2c1f133073c7c5f75ac2')

# webview.start()

# serialPort = "/dev/cu.usbserial-0001"

# baudRate = 57600

# ser = serial.Serial(serialPort, baudRate, timeout=0.5)




# while True:
# 	data = str(ser.read().decode("utf-8"))
# 	if data == 'R':
# 		data = int.from_bytes(ser.read(), "big") - 100
# 		print(data, flush=True)
# 		data = int.from_bytes(ser.read(), "big") - 100

# 	print(data, flush=True)

# ser.close()

# from curses import wrapper

# def main(stdscr):
#     # Clear screen
#     stdscr.clear()

#     # This raises ZeroDivisionError when i == 10.
#     for i in range(0, 9):
#         v = i-10
#         stdscr.addstr(i, 0, '10 divided by {} is {}'.format(v, 10/v))

#     stdscr.refresh()
#     stdscr.getkey()

# wrapper(main)


pad = curses.newpad(100, 100)
# These loops fill the pad with letters; addch() is
# explained in the next section
for y in range(0, 99):
    for x in range(0, 99):
        pad.addch(y,x, ord('a') + (x*x+y*y) % 26)

# Displays a section of the pad in the middle of the screen.
# (0,0) : coordinate of upper-left corner of pad area to display.
# (5,5) : coordinate of upper-left corner of window area to be filled
#         with pad content.
# (20, 75) : coordinate of lower-right corner of window area to be
#          : filled with pad content.
pad.refresh( 0,0, 5,5, 20,75)	