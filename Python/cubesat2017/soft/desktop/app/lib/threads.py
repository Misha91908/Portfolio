from lib import packet_parser, receiver
from lib.base import BaseThread


class TelemetryProducerThread(BaseThread):
    def __init__(
            self,
            disconnection=None,
            status=False,
            status_updater=None,
            logger_signal=None,
            clear_handler=None,
            bug_tracker_signal=None,
            *args,
            **kwargs
            ):
        super().__init__(*args, **kwargs)
        self.disconnection = disconnection
        self.status = status
        self.logger_signal = logger_signal
        self.status_updater = status_updater
        self.clear_handler = clear_handler
        self.bug_tracker_signal = bug_tracker_signal

    def run(self):
        self.is_running = True
        port = receiver.init_port_telemetry(port_number='0042', bug_tracker=self.bug_tracker_signal)
        if port is None:
            self.is_running = False
            self.disconnection()
        if self.is_running:
            self.status = True
        self.status_updater(self.send_status())
        self.logger_signal.emit()
        while self.is_running:
            packet = self.get_packet(port)
            data = packet_parser.parse_telemetry(packet, offset=4, bug_tracker=self.bug_tracker_signal)
            self.queue.put(data)
            if not self.is_running:
                self.clear_handler.emit()
            if data is ConnectionError:
                self.sleep_()
                self.clear_handler.emit()
                self.status = False
                self.status_updater(self.status)
                port.close()
                self.disconnection()

    def get_packet(self, port):
        getting_data = receiver.receive_packet_telemetry(port, self.bug_tracker_signal)
        if getting_data is not ConnectionError:
            return getting_data
        return ConnectionError

    def send_status(self):
        return self.status


class TelemetryConsumerThread(BaseThread):
    def __init__(self, tasks=None, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.tasks = tasks

    def run(self):
        self.is_running = True
        while self.is_running:
            packet = self.queue.get()
            self.queue.task_done()
            if packet is ConnectionError:
                self.is_running = False
            else:
                for do in self.tasks:
                    do(packet)


class HC12TelemetryProducerThread(BaseThread):
    def __init__(
            self,
            disconnection=None,
            status=False,
            status_updater=None,
            logger_signal=None,
            clear_handler=None,
            bug_tracker_signal=None,
            *args,
            **kwargs
            ):
        super().__init__(*args, **kwargs)
        self.disconnection = disconnection
        self.status = status
        self.logger_signal = logger_signal
        self.status_updater = status_updater
        self.clear_handler = clear_handler
        self.bug_tracker_signal = bug_tracker_signal

    def run(self):
        self.is_running = True
        port = receiver.init_port_hc12(port_number='0012', bug_tracker=self.bug_tracker_signal)
        if port is None:
            self.is_running = False
            self.disconnection()
        if self.is_running:
            self.status = True
        self.status_updater(self.send_status())
        self.logger_signal.emit()
        while self.is_running:
            packet = self.get_packet(port)
            data = packet_parser.parse_telemetry(packet, offset=4, bug_tracker=self.bug_tracker_signal)
            self.queue.put(data)
            if not self.is_running:
                self.clear_handler.emit()
            if data is ConnectionError:
                self.sleep_()
                self.clear_handler.emit()
                self.status = False
                self.status_updater(self.status)
                port.close()
                self.disconnection()

    def get_packet(self, port):
        getting_data = receiver.receive_packet_hc12(port, self.bug_tracker_signal)
        if getting_data is not ConnectionError:
            return getting_data
        return ConnectionError

    def send_status(self):
        return self.status


class HC12TelemetryConsumerThread(BaseThread):
    def __init__(self, tasks=None, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.tasks = tasks

    def run(self):
        self.is_running = True
        while self.is_running:
            packet = self.queue.get()
            self.queue.task_done()
            if packet is ConnectionError:
                self.is_running = False
            else:
                for do in self.tasks:
                    do(packet)


class APRSProducerThread(BaseThread):
    def __init__(
            self,
            status=None,
            status_updater=None,
            logger_signal=None,
            clear_handler=None,
            bug_tracker_signal=None,
            *args,
            **kwargs
            ):
        super().__init__(*args, **kwargs)
        self.status = status

    def run(self):
        port = receiver.init_port_aprs()
        while self.is_running:
            packet = self.get_packet(port)
            if packet is not None:
                data = packet_parser.parse_aprs(packet)
                self.queue.put(data)

    @staticmethod
    def get_packet(port):
        if port is not None:
            return receiver.receive_packet_aprs(port)

    def send_status(self):
        return self.status


class APRSConsumerThread(BaseThread):
    def __init__(self, tasks=None, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.tasks = tasks

    def run(self):
        pass
