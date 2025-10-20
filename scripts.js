// set year (if element exists)
const yearEl = document.getElementById('year');
if (yearEl) {
  yearEl.textContent = new Date().getFullYear();
}

// theme toggle with localStorage (safe guards if elements missing)
const storageKey = 'dp-theme';
const root = document.documentElement;
const themeToggle = document.getElementById('themeToggle');

function getTheme() {
  try {
    const s = localStorage.getItem(storageKey);
    if (s) return s;
  } catch (e) {
    // localStorage may be unavailable in some environments
  }
  return window.matchMedia && window.matchMedia('(prefers-color-scheme: dark)').matches ? 'dark' : 'light';
}

function applyTheme(t) {
  if (t === 'dark') root.setAttribute('data-theme', 'dark');
  else root.removeAttribute('data-theme');
  if (themeToggle) themeToggle.textContent = t === 'dark' ? '☀️' : '🌙';
}

if (themeToggle) {
  themeToggle.addEventListener('click', () => {
    const next = getTheme() === 'dark' ? 'light' : 'dark';
    try { localStorage.setItem(storageKey, next); } catch (e) {}
    applyTheme(next);
  });
}
applyTheme(getTheme());

// contact form fallback to mailto if user hasn't configured Formspree
const mailtoBtn = document.getElementById('mailtoBtn');
if (mailtoBtn) {
  mailtoBtn.addEventListener('click', () => {
    const nameInput = document.querySelector('input[name=name]');
    const emailInput = document.querySelector('input[name=email]');
    const msgEl = document.querySelector('textarea[name=message]');
    const name = nameInput && nameInput.value ? nameInput.value : '';
    const email = emailInput && emailInput.value ? emailInput.value : 'digambarjpatil143@gmail.com';
    const message = msgEl && msgEl.value ? msgEl.value : '';
    const body = encodeURIComponent(`From: ${name} <${email}>\n\n${message}`);
    window.location.href = `mailto:digambarjpatil143@gmail.com?subject=${encodeURIComponent('Website contact')}&body=${body}`;
  });
}

// Optional: intercept form submit if user didn't change FORM ID
const form = document.getElementById('contactForm');
if (form) {
  const action = form.getAttribute('action') || '';
  const formNotice = document.getElementById('formNotice');
  if (action.includes('YOUR_FORM_ID') || action.includes('formspree.io/f/YOUR_FORM_ID')) {
    // show a visible notice and keep submit interception
    if (formNotice) { formNotice.hidden = false }
    form.addEventListener('submit', (e) => {
      e.preventDefault();
      alert('Contact form not configured. Use the Email directly button, copy the email, or configure Formspree and replace the action URL in the HTML.');
    });
  }
}

document.addEventListener('DOMContentLoaded', () => {
  const sidebar = document.querySelector('.social-sidebar');
  if (!sidebar) return;
  const btn = sidebar.querySelector('.toggle-btn');
  if (!btn) return;

  btn.addEventListener('click', () => {
    sidebar.classList.toggle('active');
    btn.textContent = sidebar.classList.contains('active') ? '×' : '+';
  });
});

// mobile nav toggle
const navToggle = document.getElementById('navToggle');
if (navToggle) {
  navToggle.addEventListener('click', () => {
    document.body.classList.toggle('nav-open');
    navToggle.setAttribute('aria-expanded', document.body.classList.contains('nav-open'));
  });

  // close nav when navigation link clicked
  document.querySelectorAll('.nav a').forEach(a => {
    a.addEventListener('click', () => document.body.classList.remove('nav-open'));
  });

  // close on outside click
  document.addEventListener('click', (e) => {
    if (!document.body.classList.contains('nav-open')) return;
    const nav = document.querySelector('.nav');
    if (!nav) return;
    if (!nav.contains(e.target) && e.target !== navToggle) {
      document.body.classList.remove('nav-open');
    }
  });
}

// copy email to clipboard
const copyEmailBtn = document.getElementById('copyEmailBtn');
if (copyEmailBtn) {
  copyEmailBtn.addEventListener('click', async () => {
    const email = 'digambarjpatil143@gmail.com';
    try {
      await navigator.clipboard.writeText(email);
      // feedback
      const fb = document.createElement('span');
      fb.className = 'copy-feedback';
      fb.textContent = 'Copied!';
      copyEmailBtn.parentNode.appendChild(fb);
      setTimeout(() => fb.remove(), 2000);
    } catch (e) {
      alert('Copy failed — please copy manually: ' + email);
    }
  });
}
