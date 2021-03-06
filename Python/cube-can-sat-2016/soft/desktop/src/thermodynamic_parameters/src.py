import matplotlib.pyplot as plt

def reading_data():
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
    data_temp_in = [0]  # финальный список данных о температуре внутри аппарата
    data_temp_out = [0]  # финальный список данных о температуре окружающей среды
    data_pressure = [0]  # финальный список данных с датчика давления

    basic_elements = {'.', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ';'}

    for element in range(len(func_dat)-1):
        for i in range(len(func_dat[element]) - 1):
            if(func_dat[element][i] in basic_elements):
                counter += 1
        if((counter == len(func_dat[element]) - 1) and (len(func_dat[element].split(';')) - 1 == 5)):
            log_dat.append(func_dat[element])
        counter = 0


    for i in range(len(log_dat)-1):  # извлечение данных из списка и распределение их по трем другим
        element = log_dat[i].split(';')
        element[0] = float(element[1])
        element[1] = float(element[2])
        element[2] = (float(element[3]) + float(element[4]))/2.0

        try:
            data_temp_in.append(element[0])
            data_temp_out.append(element[1])
            data_pressure.append(element[2])
        except IndexError:
            data_temp_in.append(data_temp_in[-1])
            data_temp_out.append(data_temp_out[-1])
            data_pressure.append(data_pressure[-1])

    return data_temp_in, data_temp_out, data_pressure

def simple_moving_average(data_temp_in, data_temp_out, data_pressure):  # усреднение значений(простая скользящая средняя)
    lengthList1 = len(data_temp_in)
    lengthList2 = len(data_temp_out)
    lengthList3 = len(data_pressure)

    for i in range(lengthList1 - 30):  # усреднение значений
        iterator = 0
        for iterator_1 in range(30):
            iterator += 1
            data_temp_in[i] = data_temp_in[i] + data_temp_in[i + iterator]
        data_temp_in[i] = data_temp_in[i] / 30

    for i in range(lengthList2 - 30):  # усреднение значений
        iterator = 0
        for iterator_1 in range(30):
            iterator += 1
            data_temp_out[i] = data_temp_out[i] + data_temp_out[i + iterator]
        data_temp_out[i] = data_temp_out[i] / 30

    for i in range(lengthList3 - 30):  # усреднение значений
        iterator = 0
        for iterator_1 in range(30):
            iterator += 1
            data_pressure[i] = data_pressure[i] + data_pressure[i + iterator]
        data_pressure[i] = data_pressure[i] / 30

    for i in range(30):
        data_temp_in.pop()
        data_temp_out.pop()
        data_pressure.pop()


data = []
try:
    data = reading_data()
except BaseException:
    print('ошибка доступа')

data_temp_in, data_temp_out, data_pressure = sort_data(data)
simple_moving_average(data_temp_in, data_temp_out, data_pressure)

fig = plt.figure()  # графики изменения температуры внутри и снаружи аппарата и изменения давления
plt.subplot(311)
plt.plot(data_temp_in)
plt.title('Temperature inside')
plt.xticks(fontsize='10')
plt.yticks(fontsize='10')
plt.xlabel('Number of points')
plt.ylabel('temperature, celsius')
plt.grid()

plt.subplot(312)
plt.plot(data_temp_out)
plt.title('Temperature outside')
plt.xticks(fontsize='10')
plt.yticks(fontsize='10')
plt.xlabel('Number of points')
plt.ylabel('temperature, celsius')
plt.grid()

plt.subplot(313)
plt.plot(data_pressure)
plt.title('Pressure')
plt.xticks(fontsize='10')
plt.yticks(fontsize='10')
plt.xlabel('Number of points')
plt.ylabel('pressure, kPa')
plt.grid()

plt.tight_layout()
plt.savefig("../../img/termo.png", fmt='png')
