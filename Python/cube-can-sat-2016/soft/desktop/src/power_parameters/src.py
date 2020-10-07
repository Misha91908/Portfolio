import matplotlib.pyplot as plt

def reading_data_medium():
    func_dat = []
    f = open("/home/misha91908/cube-can-sat-2016/soft/desktop/log/MEDIUM.TXT", 'r')

    for line in f:
        if(line != '\n'):
            func_dat.append(line)

    return func_dat

def sort_data(func_dat):
    """
    функция, парсящая данные из списка, которые считываются в другие списки
    """
    log_dat = []
    counter = 0
    data_bat_volt = [0]  # финальный список данных о напряжении элементов питания

    basic_elements = {'.', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ';'}

    for element in range(len(func_dat)-1):
        for i in range(len(func_dat[element]) - 1):
            if(func_dat[element][i] in basic_elements):
                counter += 1
        if((counter == len(func_dat[element]) - 1) and (len(func_dat[element].split(';')) - 1 == 5)):
            log_dat.append(func_dat[element])
        counter = 0


    for i in range(len(log_dat)-1):  # извлечение данных из списка и распределение их по двум другим
        element = log_dat[i].split(';')
        element[1] = float(element[5])

        try:
            data_bat_volt.append(element[1])
        except IndexError:
            data_bat_volt.append(data_bat_volt[-1])

    return data_bat_volt

def simple_moving_average(data_bat_volt):  # усреднение значений(простая скользящая средняя)
    lengthList = len(data_bat_volt)

    for i in range(lengthList - 30):  # усреднение значений
        iterator = 0
        for iterator_1 in range(30):
            iterator += 1
            data_bat_volt[i] = data_bat_volt[i] + data_bat_volt[i + iterator]
        data_bat_volt[i] = data_bat_volt[i] / 30

    for i in range(30):
        data_bat_volt.pop()


data = []
try:
    data = reading_data_medium()
except BaseException:
    print('ошибка доступа')

data_bat_volt = sort_data(data)
simple_moving_average(data_bat_volt)

fig = plt.figure()  # графики изменения напряжения и температуры элементов питания
plt.plot(data_bat_volt)
plt.title('Battery voltage')
plt.xticks(fontsize='10')
plt.yticks(fontsize='10')
plt.xlabel('Number of points')
plt.ylabel('voltage, volts')
plt.grid()

plt.tight_layout()
plt.savefig("../../img/battery.png", fmt='png')
