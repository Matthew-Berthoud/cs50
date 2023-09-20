document.addEventListener('DOMContentLoaded', function() {
    let buttons = document.querySelectorAll('.class-btn');
    for (let i = 0; i < buttons.length; i++) {
        buttons[i].addEventListener('click', function() {
            window.location.href = `https://github.com/Matthew-Berthoud/${buttons[i].id}`;
        })
    }
});