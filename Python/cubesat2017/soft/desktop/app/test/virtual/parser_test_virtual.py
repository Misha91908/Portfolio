import sys
import pytest
import os


def test_is_valid_packet_length(packet):
    counter = 0
    valid_counter = 0
    for i in range(len(packet)):
        if len(packet_parser.parse(packet[i])) == 16:
            counter += 1
            valid_counter += 1
        else:
            counter += 1
    assert counter == valid_counter


def test_is_valid_type_of_data(packet):
    counter = 0
    valid_counter = 0
    for i in range(len(packet)):
        buf = packet_parser.parse(packet[i])
        int_count = 0
        float_count = 0
        for i in packet:
            if type(packet[i]) == int:
                int_count += 1
            elif type(packet[i]) == float:
                float_count += 1
        if int_count == 3 and float_count == 13:
            counter += 1
            valid_counter += 1
        else:
            counter += 1

    assert counter == valid_counter
