import sys
import pytest
import os


@pytest.mark.parametrize("raw", [packet])
def test_is_valid_packet_length(raw):
    assert len(packet_parser.parse(raw)) == 16


@pytest.mark.parametrize("raw", [packet])
def test_is_valid_type_of_data(raw):

    packet = packet_parser.parse(raw)
    int_count = 0
    float_count = 0
    for i in raw:
        if type(raw[i]) == int:
            int_count += 1
        elif type(raw[i]) == float:
            float_count += 1
    assert int_count == 3 and float_count == 13
