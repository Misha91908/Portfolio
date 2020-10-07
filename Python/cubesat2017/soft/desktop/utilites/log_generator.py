from random import random
import csv

out = 'log_general 27.02.2018 22:06:34.csv'  # Имя файла


def generate_packet(time):  # Генерация пакета случайным образом
    packet = []
    packet.append(time)

    for i in range(15):
        packet.append(random())

    return packet


with open(out, 'w', newline='') as csv_file:  # Запись в csv файл
    writer = csv.writer(csv_file)
    for i in range(1527339707, 1527341707):
        writer.writerow(generate_packet(i))
