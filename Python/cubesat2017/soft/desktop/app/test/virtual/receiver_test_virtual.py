import sys
import pytest
import os


def test_is_valid_number_of_bytes():

    counter = 0
    valid_counter = 0
    packet = receiver.receive_packet()
    for i in range(len(packet)):
        if len(packet[i]) == 65:
            counter += 1
            valid_counter += 1
        else:
            counter += 1
    assert counter == valid_counter


def test_is_valid_startstop_test():

    counter = 0
    valid_counter = 0
    packet = receiver.receive_packet()
    for i in range(len(packet)):
        if packet[i][0] == b'\xf1' and packet[i][len(packet[i]) - 1] == b'\xfa':
            counter += 1
            valid_counter += 1
        else:
            counter += 1

    assert counter == valid_counter


def test_is_valid_command_test():

    counter = 0
    valid_counter = 0
    packet = receiver.receive_packet()
    for i in range(len(packet)):
        if packet[i][1] == b'\xa0':
            counter += 1
            valid_counter += 1
        else:
            counter += 1

    assert counter == valid_counter
