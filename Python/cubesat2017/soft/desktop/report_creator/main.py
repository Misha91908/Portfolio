import datetime
import csv
import os
import struct
import matplotlib.pyplot as plt
import mpl_toolkits.mplot3d.axes3d as ax3d
from matplotlib.dates import DateFormatter


def parse_bin(name):
    output = []

    file_info = os.stat(name)
    print(file_info)
    with open(name, 'rb') as binfile:
        packet = binfile.read(64)
        binfile.seek(0)
        while packet:
            packet = binfile.read(64)
            packet_out = []
            if packet:
                if packet[0] == 187 and packet[-1] == 221 and packet[1] == 61:
                    packet_out.append(struct.unpack('I', packet[2:6])[0])
                    packet_out.append(struct.unpack('B', packet[10:11])[0])
                    packet_out.append(struct.unpack('I', packet[6:10])[0])
                    for j in range(11, 63, 4):
                        packet_out.append(struct.unpack('f', packet[j:j + 4])[0])
                print(packet_out)
                output.append(packet_out)

    return output


def parse_csv(name):
    output = []

    with open(name, newline='') as csvfile:
        csv_reader = csv.reader(csvfile, delimiter=',')
        for row in csv_reader:
            output.append(row)

    return output


def generate_report(data):

    time_ticks = []
    time = []
    temp = []
    ax = []
    ay = []
    az = []

    for i in range(len(data)):
        time.append(int(data[i][0]) * 100000)
        t = datetime.datetime.fromtimestamp(int(data[i][0]))
        time_ticks.append(t)
        temp.append(float(data[i][3]))
        ax.append(float(data[i][6]))
        ay.append(float(data[i][7]))
        az.append(float(data[i][8]))

    plot_data(time, temp, time_ticks)
    plot_3d_data(ax, ay, az, time)


def plot_data(x, y, x_ticks):
    list_of_gaps_number = []
    list_of_gaps = [[], []]

    for i in range(len(x) - 1):
        if x[i + 1] - x[i] > 100000:
            list_of_gaps_number.append(i)

    print(list_of_gaps_number)

    k = 0
    if len(list_of_gaps_number) > 1:
        for i in list_of_gaps_number:
            list_of_gaps[0].append(x_ticks[k:i])
            list_of_gaps[1].append(y[k:i])
            k = i + 1

        fig, ax = plt.subplots()

        for i, j in zip(list_of_gaps[0], list_of_gaps[1]):
            ax.plot(i, j, color='b')
            ax.scatter(i[0], j[0], color='r')
            ax.scatter(i[len(i) - 1], j[len(j) - 1], color='r')

        date_formatter = DateFormatter("%H:%M:%S")
        ax.xaxis.set_major_formatter(date_formatter)
        fig.autofmt_xdate()
    else:
        fig, ax = plt.subplots()

        ax.plot(x_ticks, y)

        date_formatter = DateFormatter("%H:%M:%S")
        ax.xaxis.set_major_formatter(date_formatter)
        fig.autofmt_xdate()

    plt.show()


def plot_3d_data(x, y, z, t):
    list_of_gaps_number = []
    list_of_gaps_x = []
    list_of_gaps_y = []
    list_of_gaps_z = []

    for i in range(len(x) - 1):
        if t[i + 1] - t[i] > 1:
            list_of_gaps_number.append(i)

    if len(list_of_gaps_number) > 100000:
        k = 0
        for i in list_of_gaps_number:
            list_of_gaps_x.append(x[k:i])
            list_of_gaps_y.append(y[k:i])
            list_of_gaps_z.append(z[k:i])
            k = i + 1

        fig = plt.figure()
        ax = ax3d.Axes3D(fig)

        for i, j, k in zip(list_of_gaps_x, list_of_gaps_y, list_of_gaps_z):
            ax.plot(i, j, k, color='b')
            ax.scatter(i[0], j[0], k[0], color='r')
            ax.scatter(i[-1], j[-1], k[-1], color='r')
    else:
        fig = plt.figure()
        ax = ax3d.Axes3D(fig)

        ax.plot(x, y, z)

    plt.show()


if __name__ == '__main__':
    # data_csv = parse_csv('log_telemetry_2018.05.26 13:45:09.csv')
    data_bin = parse_bin('log_cpu0.bin')
    generate_report(data_bin)
