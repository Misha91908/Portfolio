import sys
import pytest
import os


@pytest.mark.parametrize("raw", [packet])
def test_is_returning_packet(raw):
    assert type(raw) == list


@pytest.mark.parametrize("raw", [packet])
def test_is_valid_number_of_bytes(raw):
    assert len(raw) == 65


@pytest.mark.parametrize("raw", [packet])
def test_is_checksum_valid(raw):
    assert raw[len(raw) - 2] == b'\x42'


@pytest.mark.parametrize("raw", [packet])
def test_is_valid_startstop_test(raw):
    assert raw[0] == b'\xf1' and raw[len(raw) - 1] == b'\xfa'


@pytest.mark.parametrize("raw", [packet])
def test_is_valid_command_test(raw):
    assert raw[1] == b'\xa0'
