from PyQt5.QtWidgets import (QWidget, QVBoxLayout, QHBoxLayout, QPushButton, QGridLayout,
                             QLabel, QSpinBox, QDoubleSpinBox, QFrame, QErrorMessage)
from PyQt5.QtCore import Qt
from munkres import Munkres
from tkinter import *

class Matrix(QWidget):

    def __init__(self):
        super(QWidget, self).__init__()

        self.setFixedSize(1500, 550)

        self.matrixWidget = QFrame()
        self.matrixWidget.setFrameStyle(QFrame.Box)
        self.contentLayout = QHBoxLayout()
        self.buttonLayout = QVBoxLayout()
        self.MatrixGrid = QGridLayout()
        self.error_dialog = QErrorMessage()
        self.result = QWidget()

        self.MatrixConstructor = QGridLayout()
        self.MatrixConstructor.addWidget(QSpinBox(), 1, 2, Qt.AlignHCenter)
        self.MatrixConstructor.addWidget(QSpinBox(), 2, 1, Qt.AlignHCenter)
        self.MatrixConstructor.addLayout(self.MatrixGenerate(), 2, 2)

        self.MatrixConstructor.itemAt(0).widget().setRange(1, 10)
        self.MatrixConstructor.itemAt(1).widget().setRange(1, 10)
        self.MatrixConstructor.itemAt(0).widget().setValue(3)
        self.MatrixConstructor.itemAt(1).widget().setValue(3)

        item = self.MatrixConstructor.itemAt(0)
        item.widget().valueChanged.connect(self.change_size)
        item = self.MatrixConstructor.itemAt(1)
        item.widget().valueChanged.connect(self.change_size)

        self.MatrixConstructor.setAlignment(Qt.AlignCenter)
        self.matrixWidget.setLayout(self.MatrixConstructor)

        self.buttonLayout.addWidget(QLabel("Задача о назначении работников на должности."),0,Qt.AlignCenter)
        self.buttonLayout.addWidget(QPushButton('Рассчитать'), 1, Qt.AlignCenter)
        self.buttonLayout.addWidget(QWidget(), 2, Qt.AlignCenter)


        self.buttonLayout.setStretch(0, 1)
        self.buttonLayout.setStretch(2, 6)

        self.contentLayout.addWidget(self.matrixWidget, 0)
        self.contentLayout.addLayout(self.buttonLayout, 1)

        item = self.buttonLayout.itemAt(1)
        item.widget().clicked.connect(self.calculate)

        self.contentLayout.setStretch(0, 6)
        self.contentLayout.setStretch(1, 1)
        self.setLayout(self.contentLayout)

    def MatrixGenerate(self):

        for i in range(3):
            for g in range(3):
                self.MatrixGrid.addWidget(QDoubleSpinBox(), i, g, Qt.AlignHCenter)

        k = 0
        for i in range(9):
            item = self.MatrixGrid.itemAt(k)
            item.widget().setButtonSymbols(2)
            item.widget().setRange(-100000.000000, 100000.000000)
            k += 1

        return self.MatrixGrid

    def calculate(self):

        row = self.MatrixConstructor.itemAt(1).widget().value()
        column = self.MatrixConstructor.itemAt(0).widget().value()
        if row != column:
            self.error_dialog.showMessage('Ошибка, матрица должна быть квадратной')
        else:

            matrix = []
            for i in range(row):
                matrix.append([])
                for j in range(column):
                    matrix[i].append(self.MatrixGrid.itemAt(i * row + j).widget().value())
            m = Munkres()
            indexes = m.compute(matrix)
            self.result.setFixedSize(300, 200)
            layout = QVBoxLayout()

            total = 0
            for row, column in indexes:
                value = matrix[row][column]
                total += value

                layout.addWidget(QLabel('Результат рачета: (%d, %d) -> %d' % (row, column, value)))
            layout.addWidget(QLabel('Суммарный результат расчета: %d' % total))
            self.result.setLayout(layout)
            self.result.show()

    def change_size(self):
        if self.MatrixConstructor:
            while (self.MatrixGrid.count()):
                item = self.MatrixGrid.takeAt(0)
                widget = item.widget()
                if widget is not None:
                    widget.deleteLater()

            row = self.MatrixConstructor.itemAt(1).widget().value()
            column = self.MatrixConstructor.itemAt(0).widget().value()

            for i in range(row):
                for g in range(column):
                    self.MatrixGrid.addWidget(QDoubleSpinBox(), i, g)

            for i in range(row * column):
                item = self.MatrixGrid.itemAt(i)
                item.widget().setButtonSymbols(2)
                item.widget().setRange(-100000.000000, 100000.000000)

        elif self.MatrixBase:
            while (self.MatrixGrid.count()):
                item = self.MatrixGrid.takeAt(0)
                widget = item.widget()
                if widget is not None:
                    widget.deleteLater()

            row = self.MatrixBase.itemAt(1).widget().value()
            column = self.MatrixBase.itemAt(0).widget().value()

            for i in range(row):
                for g in range(column):
                    self.MatrixGrid.addWidget(QDoubleSpinBox(), i, g)

            for i in range(row * column):
                item = self.MatrixGrid.itemAt(i)
                item.widget().setButtonSymbols(2)
                item.widget().setRange(-100000.000000, 100000.000000)

            while (self.MatrixValues.count()):
                item = self.MatrixValues.takeAt(0)
                widget = item.widget()
                if widget is not None:
                    widget.deleteLater()

            for i in range(row):
                self.MatrixValues.addWidget(QDoubleSpinBox())
                item = self.MatrixValues.itemAt(i)
                item.widget().setButtonSymbols(2)
                item.widget().setRange(-100000.000000, 100000.000000)

    def clearMatrix(self):

        row = self.MatrixGrid.rowCount()
        column = self.MatrixGrid.columnCount()

        for i in range(row * column):
            item = self.MatrixGrid.itemAt(i)
            item.widget().setValue(0.0)

        if type == 1:
            for i in range(self.MatrixValues.count()):
                item = self.MatrixValues.itemAt(i)
                item.widget().setValue(0.0)
