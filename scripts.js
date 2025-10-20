// set year
document.getElementById('year').textContent = new Date().getFullYear();

// theme toggle with localStorage
const storageKey = 'dp-theme';
const root = document.documentElement;
const themeToggle = document.getElementById('themeToggle');

function getTheme(){
  const s = localStorage.getItem(storageKey);
  if(s) return s;
  return window.matchMedia && window.matchMedia('(prefers-color-scheme: dark)').matches ? 'dark' : 'light';
}
function applyTheme(t){
  if(t === 'dark') root.setAttribute('data-theme','dark');
  else root.removeAttribute('data-theme');
  themeToggle.textContent = t === 'dark' ? '☀️' : '🌙';
}
themeToggle.addEventListener('click', ()=>{
  const next = getTheme() === 'dark' ? 'light' : 'dark';
  localStorage.setItem(storageKey, next);
  applyTheme(next);
});
applyTheme(getTheme());

// contact form fallback to mailto if user hasn't configured Formspree
document.getElementById('mailtoBtn').addEventListener('click', ()=>{
  const name = document.querySelector('input[name=name]').value || '';
  const email = document.querySelector('input[name=email]').value || 'digambarjpatil143@gmail.com';
  const message = document.querySelector('textarea[name=message]').value || '';
  const body = encodeURIComponent(`From: ${name} <${email}>\n\n${message}`);
  window.location.href = `mailto:digambarjpatil143@gmail.com?subject=${encodeURIComponent('Website contact')}&body=${body}`;
});

// Optional: intercept form submit if user didn't change FORM ID
const form = document.getElementById('contactForm');
form.addEventListener('submit', (e)=>{
  const action = form.getAttribute('action') || '';
  if(action.includes('YOUR_FORM_ID') || action.includes('formspree.io/f/YOUR_FORM_ID')){
    e.preventDefault();
    alert('Contact form not configured. Use the Email directly button or configure Formspree and replace the action URL in the HTML.');
  }
});

document.addEventListener('DOMContentLoaded', () => {
  const sidebar = document.querySelector('.social-sidebar');

  // Check if toggle button already exists
  if (!sidebar.querySelector('.toggle-btn')) {
    const btn = document.createElement('button');
    btn.textContent = '+';
    btn.className = 'toggle-btn';
    btn.style.cssText = 'width:50px;height:50px;border-radius:50%;background:linear-gradient(135deg,#a4508b,#5f0a87);color:white;font-size:24px;cursor:pointer;margin-top:10px;';
    sidebar.appendChild(btn);

    btn.addEventListener('click', () => {
      sidebar.classList.toggle('active');
      btn.textContent = sidebar.classList.contains('active') ? '×' : '+';
    });
  }
});

