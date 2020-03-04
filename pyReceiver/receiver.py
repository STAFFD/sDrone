import serial
from curses import wrapper

def main(stdscr):
    # Clear screen

    serialPort = "/dev/cu.usbserial-0001"
    baudRate = 57600
    ser = serial.Serial(serialPort, baudRate, timeout=0.5)
    stdscr.clear()

    while True:

        try:
            data = str(ser.read().decode("utf-8"))
        except Exception:
            pass

        if data == 'R':
            angle_pitch = int.from_bytes(ser.read(), "big") - 100
            angle_roll = int.from_bytes(ser.read(), "big") - 100
            angle_yaw = int.from_bytes(ser.read(), "big") - 100

            stdscr.addstr(0, 0, 'angle_pitch {} | angle_roll {} | angle_yaw {}'.format(angle_pitch, angle_roll, angle_yaw))

        stdscr.refresh()


if __name__ == '__main__':
    wrapper(main)