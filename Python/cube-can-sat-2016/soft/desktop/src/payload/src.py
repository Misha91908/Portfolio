import matplotlib.pyplot as plt

def reading_data():
    func_dat = []
    f = open("/home/misha91908/cube-can-sat-2016/soft/desktop/log/LOG.TXT", 'r')

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
    data_1_state = [0]  # финальный список данных АЦП в первом режиме включения
    data_2_state = [0]  # финальный список данных АЦП во втором режиме включения
    data_3_state = [0]  # финальный список данных АЦП в тертьем режиме включения

    basic_elements = {'.', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ';'}

    for element in range(len(func_dat)-1):
        for i in range(len(func_dat[element]) - 1):
            if(func_dat[element][i] in basic_elements):
                counter += 1
        if((counter == len(func_dat[element]) - 1) and (len(func_dat[element].split(';')) - 1 == 2)):
            log_dat.append(func_dat[element])
        counter = 0


    for i in range(len(log_dat)-1):  # извлечение данных из списка и распределение их по двум другим
        element = log_dat[i].split(';')
        if (element[0] == '0'):
            try:
                data_1_state.append((3.3 / 0xFFFFFFFF) * float(element[1]))
            except IndexError:
                data_1_state.append(data_1_state[-1])
        elif (element[0] == '1'):
            try:
                data_2_state.append((3.3 / 0xFFFFFFFF) * float(element[1]))
            except IndexError:
                data_2_state.append(data_2_state[-1])
        elif (element[0] == '2'):
            try:
                data_3_state.append((3.3 / 0xFFFFFFFF) * float(element[1]))
            except IndexError:
                data_3_state.append(data_3_state[-1])

    return data_1_state, data_2_state, data_3_state

def simple_moving_average(data_1_state, data_2_state, data_3_state):  # усреднение значений(простая скользящая средняя)
    lengthList1 = len(data_1_state)
    lengthList2 = len(data_2_state)
    lengthList3 = len(data_3_state)

    for i in range(lengthList1 - 30):  # усреднение значений
        iterator = 0
        for iterator_1 in range(30):
            iterator += 1
            data_1_state[i] = data_1_state[i] + data_1_state[i + iterator]
        data_1_state[i] = data_1_state[i] / 30

    for i in range(lengthList2 - 30):  # усреднение значений
        iterator = 0
        for iterator_1 in range(30):
            iterator += 1
            data_2_state[i] = data_2_state[i] + data_2_state[i + iterator]
        data_2_state[i] = data_2_state[i] / 30

    for i in range(lengthList3 - 30):  # усреднение значений
        iterator = 0
        for iterator_1 in range(30):
            iterator += 1
            data_3_state[i] = data_3_state[i] + data_3_state[i + iterator]
        data_3_state[i] = data_3_state[i] / 30

    for i in range(29):
        data_1_state.pop()
        data_2_state.pop()
        data_3_state.pop()

data = []

try:
    data = reading_data()
except BaseException:
    print("Ошибка доступа")

data_1_state, data_2_state, data_3_state = sort_data(data)
simple_moving_average(data_1_state, data_2_state, data_3_state)

fig = plt.figure()  # графики изменения данных с АЦП
plt.subplot(311)
plt.plot(data_1_state)
plt.title('Data on 1 state')
plt.xticks(fontsize='10')
plt.yticks(fontsize='10')
plt.xlabel('Number of points')
plt.ylabel('voltage, volts')
plt.grid()

plt.subplot(312)
plt.plot(data_2_state)
plt.title('Data on 2 state')
plt.xticks(fontsize='10')
plt.yticks(fontsize='10')
plt.xlabel('Number of points')
plt.ylabel('voltage, volts')
plt.grid()

plt.subplot(313)
plt.plot(data_3_state)
plt.title('Data on 3 state')
plt.xticks(fontsize='10')
plt.yticks(fontsize='10')
plt.xlabel('Number of points')
plt.ylabel('voltage, volts')
plt.grid()

plt.tight_layout()
plt.savefig("../../img/payload.png", fmt='png')
