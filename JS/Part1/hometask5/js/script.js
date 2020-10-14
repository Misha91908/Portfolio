'use_strict';

let menu = document.getElementsByClassName("menu")[0],
    menu_buttons = document.getElementsByClassName('menu-item'),
    header = document.getElementById('title'),
    adv = document.getElementsByClassName('adv')[0],
    prompt_item = document.getElementById('prompt');

menu.insertBefore(menu_buttons[2], menu_buttons[1]);

header.textContent = 'Мы продаем только подлинную технику Apple';
document.body.style.backgroundImage = "url('img/apple_true.jpg')";

adv.remove();

let ans = prompt('Как вы относитесь к технике Apple?', '');

prompt_item.textContent = ans;
