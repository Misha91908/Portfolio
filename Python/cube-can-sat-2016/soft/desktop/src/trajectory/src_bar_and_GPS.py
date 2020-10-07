import matplotlib.pyplot as plt
import mpl_toolkits.mplot3d.axes3d as ax3d
from math import sqrt
import itertools

def reading_mediuum_data():
    func_dat = []
    f = open("/home/misha91908/cube-can-sat-2016/soft/desktop/log/MEDIUM.TXT", 'r')

    for line in f:
        if(line != '\n'):
            func_dat.append(line)

def reading_low_data():
    func_dat = []
    f = open("/home/misha91908/cube-can-sat-2016/soft/desktop/log/FAST.TXT", 'r')

    for line in f:
        if(line != '\n'):
            func_dat.append(line)

    return func_dat

def sort_medium_data(func_dat):
    """
    функция, парсящая данные из списка, которые считываются в другие списки
    """
    log_dat = []
    counter = 0
    data_meters = [0]  # финальный список данных высоты по барометру

    basic_elements = {'.', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ';'}

    for element in range(len(func_dat)-1):
        for i in range(len(func_dat[element]) - 1):
            if(func_dat[element][i] in basic_elements):
                counter += 1
        if((counter == len(func_dat[element]) - 1) and (len(func_dat[element].split(';')) - 1 == 10)):
            log_dat.append(func_dat[element])
        counter = 0


    for i in range(len(log_dat)-2):  # извлечение данных из списка и распределение их по двум другим
        element = log_dat[i].split(';')
        element[0] = (float(element[3]) + float(element[4]))/2
        # Z = 18336 метров (1+0,002845cos2φ)·[1+(t+t1)/500]·log(H/h)
        try:
            data_meters.append(element[0])
        except IndexError:
            data_meters.append(data_meters[-1])

    return data_meters

def sort_low_data(func_dat):
    """
    функция, парсящая данные из списка, которые считываются в другие списки
    """
    log_dat = []
    counter = 0
    datax = [0]  # финальный список абсцисс
    datay = [0]  # финальный список ординат
    dataz = [0]  # финальный список аппликат

    basic_elements = {'-', '.', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ';'}

    for element in range(len(func_dat)-1):
        for i in range(len(func_dat[element]) - 1):
            if(func_dat[element][i] in basic_elements):
                counter += 1
        if((counter == len(func_dat[element]) - 1) and (len(func_dat[element].split(';')) - 1 == 9)):
            log_dat.append(func_dat[element])
        counter = 0


    for i in range(len(log_dat)-1):  # извлечение данных из списка и распределение их по трем другим
        element = log_dat[i].split(';')
        element[0] = float(element[1])
        element[1] = float(element[2])
        element[2] = float(element[3])

        try:
            datax.append(element[0])
            datay.append(element[1])
            dataz.append(element[2])
        except IndexError:
            datax.append(datax[-1])
            datay.append(datay[-1])
            dataz.append(dataz[-1])

    return datax, datay, dataz

def simple_moving_average(func_data_x, func_data_y, func_data_z):  # усреднение значений(простая скользящая средняя)
    lengthList = len(func_data_x)

    for i in range(lengthList - 45):  # усреднение значений
        iterator = 0
        for iterator_1 in range(45):
            iterator += 1
            func_data_x[i] = func_data_x[i] + func_data_x[i + iterator]
            func_data_y[i] = func_data_y[i] + func_data_y[i + iterator]
            func_data_z[i] = func_data_z[i] + func_data_z[i + iterator]
        func_data_x[i] = func_data_x[i] / 30
        func_data_y[i] = func_data_y[i] / 30
        func_data_z[i] = func_data_z[i] / 30


def integration(*axis, N=1):
    for _ in range(N):
        axis = map(lambda x: list(itertools.accumulate(x)), axis)

    return list(axis)


dat = []
dat1 = []
try:
    dat = reading_low_data()
except BaseException:
    print('ошибка доступа')

try:
    dat1 = reading_low_data()
except BaseException:
    print('ошибка доступа')


data_x, data_y, data_z = sort_low_data(dat)
simple_moving_average(data_x, data_y, data_z)
data_x, data_y, data_z = integration(data_x, data_y, data_z, N=2)
length_list = len(data_x)

data_meters = sort_medium_data(dat1)

fig = plt.figure()  # график траектории перемещения
ax = ax3d.Axes3D(fig)
ax.plot(data_x, data_y, data_z, color='green')
ax.scatter(data_x[0], data_y[0], data_z[0])
ax.scatter(data_x[length_list - 1], data_y[length_list - 1], data_z[length_list - 1], color='red')
plt.savefig("../../img/trajectory_GPS.png", fmt='png')

fig1 = plt.figure()  # графики изменения проекций ускорения по 3 осям
plt.subplot(311)
plt.plot(data_x)
plt.title('Latitude')
plt.xticks(fontsize='10')
plt.yticks(fontsize='10')
plt.xlabel('Number of points')
plt.ylabel('Degrees')
plt.grid()

plt.subplot(312)
plt.plot(data_y)
plt.title('Longitude')
plt.xticks(fontsize='10')
plt.yticks(fontsize='10')
plt.xlabel('Number of points')
plt.ylabel('Degrees')
plt.grid()

plt.subplot(313)
plt.plot(data_z)
plt.title('Altitude')
plt.xticks(fontsize='10')
plt.yticks(fontsize='10')
plt.xlabel('Number of points')
plt.ylabel('Meters')
plt.grid()

plt.tight_layout()
plt.savefig("../../img/GPS_data.png", fmt='png')

fig2 = plt.figure()
plt.plot(data_meters)
plt.title('Altitude')
plt.xticks(fontsize='10')
plt.yticks(fontsize='10')
plt.xlabel('Number of points')
plt.ylabel('Meters')
plt.grid()

plt.tight_layout()
plt.savefig("../../img/bar_alt.png", fmt='png')
