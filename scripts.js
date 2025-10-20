// -----------------------------
// Footer Year
// -----------------------------
const yearEl = document.getElementById('year');
if (yearEl) {
  yearEl.textContent = new Date().getFullYear();
}

// -----------------------------
// Theme Toggle with localStorage
// -----------------------------
const storageKey = 'dp-theme';
const root = document.documentElement;
const themeToggle = document.getElementById('themeToggle');

function getTheme() {
  try {
    const s = localStorage.getItem(storageKey);
    if (s) return s;
  } catch (e) {}
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

// -----------------------------
// Mobile Navigation Toggle
// -----------------------------
const navToggle = document.getElementById('navToggle');
if (navToggle) {
  navToggle.addEventListener('click', () => {
    document.body.classList.toggle('nav-open');
    navToggle.setAttribute('aria-expanded', document.body.classList.contains('nav-open'));
  });

  document.querySelectorAll('.nav a').forEach(a => {
    a.addEventListener('click', () => document.body.classList.remove('nav-open'));
  });

  document.addEventListener('click', (e) => {
    if (!document.body.classList.contains('nav-open')) return;
    const nav = document.querySelector('.nav');
    if (!nav) return;
    if (!nav.contains(e.target) && e.target !== navToggle) {
      document.body.classList.remove('nav-open');
    }
  });
}

// -----------------------------
// Social Sidebar Toggle
// -----------------------------
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

// -----------------------------
// Contact Form Handling
// -----------------------------
const form = document.getElementById('contactForm');
const formNotice = document.getElementById('formNotice');
const email = 'digambarjpatil143@gmail.com';

if (form) {
  const action = form.getAttribute('action') || '';

  // If Formspree is configured
  if (!action.includes('YOUR_FORM_ID')) {
    form.addEventListener('submit', async (e) => {
      e.preventDefault();
      const formData = new FormData(form);
      try {
        const response = await fetch(action, {
          method: "POST",
          body: formData,
          headers: { "Accept": "application/json" },
        });
        if (response.ok) {
          if (formNotice) {
            formNotice.hidden = false;
            formNotice.textContent = "Thanks! Your message has been sent.";
            formNotice.style.color = "green";
          }
          form.reset();
        } else {
          if (formNotice) {
            formNotice.hidden = false;
            formNotice.textContent = "Oops! Something went wrong. Try emailing directly.";
            formNotice.style.color = "red";
          }
        }
      } catch (error) {
        if (formNotice) {
          formNotice.hidden = false;
          formNotice.textContent = "Network error. Please email directly.";
          formNotice.style.color = "red";
        }
      }
    });
  } else {
    // Formspree not configured: show notice and alert on submit
    if (formNotice) formNotice.hidden = false;
    form.addEventListener('submit', (e) => {
      e.preventDefault();
      alert('Contact form not configured. Use the Email directly button, copy the email, or configure Formspree and replace the action URL in the HTML.');
    });
  }
}

// Mailto button fallback
const mailtoBtn = document.getElementById('mailtoBtn');
if (mailtoBtn) {
  mailtoBtn.addEventListener('click', () => {
    const nameInput = document.querySelector('input[name=name]');
    const emailInput = document.querySelector('input[name=email]');
    const msgEl = document.querySelector('textarea[name=message]');
    const nameVal = nameInput && nameInput.value ? nameInput.value : '';
    const emailVal = emailInput && emailInput.value ? emailInput.value : email;
    const messageVal = msgEl && msgEl.value ? msgEl.value : '';
    const body = encodeURIComponent(`From: ${nameVal} <${emailVal}>\n\n${messageVal}`);
    window.location.href = `mailto:${email}?subject=${encodeURIComponent('Website contact')}&body=${body}`;
  });
}

// Copy email button
const copyEmailBtn = document.getElementById('copyEmailBtn');
if (copyEmailBtn) {
  copyEmailBtn.addEventListener('click', async () => {
    try {
      await navigator.clipboard.writeText(email);
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
const successMsg = document.getElementById('successMessage');
successMsg.style.display = 'block'; // show message

// auto-hide after 3 seconds
setTimeout(() => {
    successMsg.classList.add('hide');
}, 3000);
