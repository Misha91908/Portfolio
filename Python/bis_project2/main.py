import sys
from PyQt5.QtWidgets import QApplication
from src.Matrix import Matrix


if __name__ == "__main__":
    app = QApplication(sys.argv)

    w = Matrix()
    w.show()

    sys.exit(app.exec_())