from PyQt5 import QtCore, QtWidgets
import lib.exeptions

template_telemetry = '''================================================================================
                Packet ID:                     \t\t{packet_id}
                Number Of Satellites:          \t\t{sats}
                Time:                          \t\t{time}
                Temperature Inside Apparatus:  \t{temp_in} \t\u2103 
                Temperature Outside Apparatus: \t{temp_out} \t\u2103 
                Pressure:                      \t\t{pressure}\t kPa
                Axelleration(OX):              \t\t{axel_x}
                Axelleration(OY):              \t\t{axel_y}
                Axelleration(OZ):              \t\t{axel_z}
                Latitude:                      \t\t{lat}
                Longitude:                     \t\t{lon}
                Altitude:                      \t\t{alt}\t m
                HDOP:                          \t\t{hdop}
                VDOP:                          \t\t{vdop}
                Battery Voltage:               \t\t{bat_volt}\t V
                Battery Current:               \t\t{bat_amp}\t A \n'''

template_APRS = '''================================================================================
                Packet ID:                     \t\t{packet_id}
                Number Of Satellites:          \t\t{sats}
                Time:                          \t\t{time}
                Temperature Inside Apparatus:  \t{temp_in} \t\u2103 
                Temperature Outside Apparatus: \t{temp_out} \t\u2103 
                Pressure:                      \t\t{pressure}\t kPa
                Latitude:                      \t\t{lat}
                Longitude:                     \t\t{lon}
                Altitude:                      \t\t{alt}\t m
                Battery Voltage:               \t\t{bat_volt}\t V
                Battery Current:               \t\t{bat_amp}\t A \n'''


class LogWindow(QtWidgets.QTextEdit):
    update_signal = QtCore.pyqtSignal(dict, type)

    def __init__(self):
        super().__init__()

        self.init_ui()

    def init_ui(self):
        self.setTextInteractionFlags(QtCore.Qt.NoTextInteraction)
        self.setLineWrapMode(QtWidgets.QTextEdit.NoWrap)

    def update_logger(self, packet):
        if isinstance(packet, type):
            self.update_signal.emit({}, packet)
        elif isinstance(packet, dict):
            self.update_signal.emit(packet, lib.exeptions.NoExeptions)
