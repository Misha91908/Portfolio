import matplotlib.pyplot as plt
import mpl_toolkits.mplot3d.axes3d as ax3d
from math import sqrt
import itertools

def reading_data():
    func_dat = []
    f = open("/home/misha91908/cube-can-sat-2016/soft/desktop/log/FAST.TXT", 'r')

    for line in f:
        if(line != '\n'):
            func_dat.append(line)

    return func_dat

def sort_data(func_dat):
    """
    функция, парсящая данные из списка, которые считываются в другие списки по осям
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
        if((counter == len(func_dat[element]) - 1) and (len(func_dat[element].split(';')) - 1 == 2)):
            log_dat.append(func_dat[element])
        counter = 0


    for i in range(len(log_dat)-1):  # извлечение данных из списка и распределение их по трем другим
        element = log_dat[i].split(';')
        element[0] = float(element[0]) * 0.0000000598145
        element[1] = float(element[1]) * 0.0000000598145
        element[2] = float(element[2]) * 0.0000000598145

        try:
            datax.append(element[0])
            datay.append(element[1])
            dataz.append(element[2])
        except IndexError:
            datax.append(datax[-1])
            datay.append(datay[-1])
            dataz.append(dataz[-1])

    return datax, datay, dataz


def calculating_the_distance(func_data_x, func_data_y, func_data_z):  # функция, рассчитывающая длину тракетории
    track = 0.0  # длина траектории перемещения
    length = 0.0  # расстояние между двумя соседними точками

    for i in range(len(func_data_x) - 2):  # вычисление длины траектории перемещения
        vector_x = func_data_x[i + 1] - func_data_x[i]
        vector_y = func_data_y[i + 1] - func_data_y[i]
        vector_z = func_data_z[i + 1] - func_data_z[i]
        length = sqrt(vector_x * vector_x + vector_y * vector_y + vector_z * vector_z)
        track = track + length
    return track


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
try:
    dat = reading_data()
except BaseException:
    print('ошибка доступа')

data_x, data_y, data_z = sort_data(dat)
simple_moving_average(data_x, data_y, data_z)
data_x, data_y, data_z = integration(data_x, data_y, data_z, N=2)
length_list = len(data_x)

print("Длина траектории = ", calculating_the_distance(data_x, data_y, data_z))

fig = plt.figure()  # график траектории перемещения
ax = ax3d.Axes3D(fig)
ax.plot(data_x, data_y, data_z, color='green')
ax.scatter(data_x[0], data_y[0], data_z[0])
ax.scatter(data_x[length_list - 1], data_y[length_list - 1], data_z[length_list - 1], color='red')
plt.savefig("../../img/trajectory.png", fmt='png')

fig1 = plt.figure()  # графики изменения проекций ускорения по 3 осям
plt.subplot(311)
plt.plot(data_x)
plt.title('Axelleration(x)')
plt.xticks(fontsize='10')
plt.yticks(fontsize='10')
plt.xlabel('Number of points')
plt.ylabel('Distance, meters')
plt.grid()

plt.subplot(312)
plt.plot(data_y)
plt.title('Axelleration(y)')
plt.xticks(fontsize='10')
plt.yticks(fontsize='10')
plt.xlabel('Number of points')
plt.ylabel('Distance, meters')
plt.grid()

plt.subplot(313)
plt.plot(data_z)
plt.title('Axelleration(z)')
plt.xticks(fontsize='10')
plt.yticks(fontsize='10')
plt.xlabel('Number of points')
plt.ylabel('Distance, meters')
plt.grid()

plt.tight_layout()
plt.savefig("../../img/XYZprojection.png", fmt='png')
