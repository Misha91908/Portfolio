import matplotlib.pyplot as plt

def reading_low_data():
    func_dat = []
    f = open("/home/misha91908/cube-can-sat-2016/soft/desktop/log/LOW.TXT", 'r')

    for line in f:
        if(line != '\n'):
            func_dat.append(line)

    return func_dat

def sort_low_data(func_dat):
    """
    функция, парсящая данные из списка, которые считываются в другие списки
    """
    log_dat = []
    counter = 0
    data_dust = [0]  # финальный список данных о кол-ве пыли
    data_ozone = [0]  # финальный список данных о кол-ве озона

    basic_elements = {'.', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ';'}

    for element in range(len(func_dat)-1):
        for i in range(len(func_dat[element]) - 1):
            if(func_dat[element][i] in basic_elements):
                counter += 1
        if((counter == len(func_dat[element]) - 1) and (len(func_dat[element].split(';')) - 1 == 8)):
            log_dat.append(func_dat[element])
        counter = 0


    for i in range(len(log_dat)-1):  # извлечение данных из списка и распределение их по двум другим
        element = log_dat[i].split(';')
        element[0] = float(element[6])
        element[1] = float(element[7])

        try:
            data_dust.append(element[0])
            data_ozone.append(element[1])
        except IndexError:
            data_dust.append(data_dust[-1])
            data_ozone.append(data_ozone[-1])

    return data_dust, data_ozone

def simple_moving_average(data1, data2):  # усреднение значений(простая скользящая средняя)
    lengthList1 = len(data1)
    lengthList2 = len(data2)

    for i in range(lengthList1 - 30):  # усреднение значений
        iterator = 0
        for iterator_1 in range(30):
            iterator += 1
            data1[i] = data1[i] + data1[i + iterator]
        data1[i] = data1[i] / 30

    for i in range(lengthList2 - 30):  # усреднение значений
        iterator = 0
        for iterator_1 in range(30):
            iterator += 1
            data2[i] = data2[i] + data2[i + iterator]
        data2[i] = data2[i] / 30

    for i in range(30):
        data1.pop()
        data2.pop()


data = []

try:
    data = reading_low_data()
except BaseException:
    print('ошибка доступа')

data_dust, data_ozone = sort_low_data(data)
simple_moving_average(data_dust, data_ozone)

fig = plt.figure()  # графики изменения количества пыли и озона в атмосфере
plt.subplot(211)
plt.plot(data_dust)
plt.title('Dust')
plt.xticks(fontsize='10')
plt.yticks(fontsize='10')
plt.xlabel('Number of points')
plt.ylabel('quantity, g/sm^3')
plt.grid()

plt.subplot(212)
plt.plot(data_ozone)
plt.title('Ozone')
plt.xticks(fontsize='10')
plt.yticks(fontsize='10')
plt.xlabel('Number of points')
plt.ylabel('quantity, g/sm^3')
plt.grid()

plt.tight_layout()
plt.savefig("../../img/Dust_Ozone.png", fmt='png')
