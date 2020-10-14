let startBtn = document.getElementById("start"),
    budgetValue = document.getElementsByClassName('budget-value')[0],
    dayBudgetValue = document.getElementsByClassName('daybudget-value')[0],
    levelValue = document.getElementsByClassName('level-value')[0],
    expensesValue = document.getElementsByClassName('expenses-value')[0],
    optionalExpensesValue = document.getElementsByClassName('optionalexpenses-value')[0],
    incomeValue = document.getElementsByClassName('income-value')[0],
    monthSavingsValue = document.getElementsByClassName('monthsavings-value')[0],
    yearSavingsValue = document.getElementsByClassName('yearsavings-value')[0],


    expensesItem = document.getElementsByClassName('expenses-item'),
    expensesBtn = document.getElementsByTagName('button')[0],
    optionalExpensesBtn = document.getElementsByTagName('button')[1],
    countBtn = document.getElementsByTagName('button')[2],
    optionalExpensesItem = document.querySelectorAll('.optionalexpenses-item'),
    incomeItem = document.querySelector('.choose-income'),
    checkSavings = document.querySelector('#savings'),
    sumValue = document.querySelector('.choose-sum'),
    percentValue = document.querySelector('.choose-percent'),
    yearValue = document.querySelector('.year-value'),
    monthValue = document.querySelector('.month-value'),
    dayValue = document.querySelector('.day-value');

let money, time;

expensesBtn.disabled = true;
optionalExpensesBtn.disabled = true;
countBtn.disabled = true;

startBtn.addEventListener('click', () => {
    time = prompt('Введите дату в формате YYYY-MM-DD', '');
    money = +prompt("Ваш бюджет на месяц?", '');

    while (isNaN(money) || money === '' || money == null) {
        money = prompt("Ваш бюджет?", "");
    }
    appData.budget = money;
    appData.timeData = time;
    budgetValue.textContent = money.toFixed();

    if (!isNaN(Date.parse(time))) {
        yearValue.value = new Date(Date.parse(time)).getFullYear();
        monthValue.value = new Date(Date.parse(time)).getMonth() + 1;
        dayValue.value = new Date(Date.parse(time)).getDate();
    } else {
        yearValue.value = 'YYYY';
        monthValue.value = 'MM';
        dayValue.value = 'DD';
    }

    expensesBtn.disabled = false;
    optionalExpensesBtn.disabled = false;
    countBtn.disabled = false;
});

expensesBtn.addEventListener('click', () => {
    let sum = 0;
    for (let i = 0; i < expensesItem.length; i++) {
        let a = expensesItem[i].value,
            b = +expensesItem[++i].value;
        console.log(expensesItem[i].value);
        if ((typeof (a)) != null && (typeof (b)) != null && a !== '' && !isNaN(b) && a.length < 50) {
            appData.expenses[a] = b;
            sum += b;
        } else {
            alert('Ошибка ввода данных, пожалуйста, повторите действие \n\n' +
                'Справка: поле Наименование не может быть пустым, а в поле Цена можно вводить только числа, ' +
                'для дробного разделителя использовать точку');
            break;
        }
        expensesValue.textContent = sum;
        if (appData.moneyPerDay != null || appData.moneyPerDay !== undefined) {
            countBtn.dispatchEvent(new MouseEvent('click'));
        }
    }
});

optionalExpensesBtn.addEventListener('click', () => {
    optionalExpensesValue.textContent = '';
    for (let i = 0; i < optionalExpensesItem.length; i++) {
        if (optionalExpensesItem[i].value !== '') {
            appData.optionalExpenses[i] = optionalExpensesItem[i].value;
            optionalExpensesValue.textContent += appData.optionalExpenses[i] + ' ';
        }
    }
});

countBtn.addEventListener('click', () => {
    if (appData.budget !== undefined) {
        appData.moneyPerDay = ((appData.budget - +expensesValue.textContent) / 30).toFixed();
        dayBudgetValue.textContent = appData.moneyPerDay;
        if (appData.moneyPerDay < 100) {
            levelValue.textContent = 'Минимальный уровень достатка';
        } else if (appData.moneyPerDay > 100 && appData.moneyPerDay < 2000) {
            levelValue.textContent = 'Средний уровень достатка';
        } else if (appData.moneyPerDay > 2000) {
            levelValue.textContent = 'Высокий уровень достатка';
        } else {
            levelValue.textContent = 'Произошла ошибка';
        }
    } else {
        dayBudgetValue.textContent = 'Произошла ошибка';
    }
});

incomeItem.addEventListener('input', () => {
    let items = incomeItem.value;
    if (isNaN(items) || items !== '') {
        appData.income = items.split(',');
        incomeValue.textContent = appData.income;
    }
});

checkSavings.addEventListener("click", () => {
    appData.savings = appData.savings !== true;
});

sumValue.addEventListener('input', () => {
    if (appData.savings === true) {
        let sum = +sumValue.value;
        let percent = +percentValue.value;
        if (!isNaN(sum) && !isNaN(percent)) {
            appData.monthIncome = sum / 100 / 12 * percent;
            appData.yearIncome = sum / 100 * percent;
            monthSavingsValue.textContent = appData.monthIncome.toFixed(1);
            yearSavingsValue.textContent = appData.yearIncome.toFixed(1);
        } else {
            alert('Ошибка ввода данных, пожалуйста, повторите действие \n\n' +
                'Справка: можно вводить только числа, для дробного разделителя использовать точку');
            sumValue.textContent = sumValue.value.replace('/\w/', '');
            percentValue.textContent = sumValue.value.replace('/\w/', '');
            monthSavingsValue.textContent = 'Ошибка ввода';
            yearSavingsValue.textContent = 'Ошибка ввода';
        }
    }
});

percentValue.addEventListener('input', () => {
    if (appData.savings === true) {
        let sum = +sumValue.value;
        let percent = +percentValue.value;
        if (!isNaN(sum) && !isNaN(percent)) {
            appData.monthIncome = sum / 100 / 12 * percent;
            appData.yearIncome = sum / 100 * percent;
            monthSavingsValue.textContent = appData.monthIncome.toFixed(1);
            yearSavingsValue.textContent = appData.yearIncome.toFixed(1);
        } else {
            alert('Ошибка ввода данных, пожалуйста, повторите действие \n\n' +
                'Справка: можно вводить только числа, для дробного разделителя использовать точку');
            sumValue.textContent = sumValue.value.replace('/\w/', '');
            percentValue.textContent = sumValue.value.replace('/\w/', '');
            monthSavingsValue.textContent = 'Ошибка ввода';
            yearSavingsValue.textContent = 'Ошибка ввода';
        }
    }
});

const appData = {
    budget: money,
    expenses: {},
    optionalExpenses: {},
    income: [],
    timeData: time,
    savings: false
};
