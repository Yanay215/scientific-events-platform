<template>
  <div class="form-wrapper">
    <div class="form-container">
      <header class="form-header">
        <h1>Придумайте<br>новый пароль</h1>
      </header>

      <div v-if="globalError" class="global-error-message">
        {{ globalError }}
      </div>

      <form @submit.prevent="handleSubmit" class="main-form">
        <div class="strength-indicator">
          <div class="strength-label">Сложность пароля:</div>
          <div class="strength-visual">
            <div class="strength-bar-bg">
              <div class="strength-bar-fill" :style="passwordStrength.style"></div>
            </div>
            <span class="strength-text" v-if="passwordStrength.text">{{ passwordStrength.text }}</span>
          </div>
        </div>

        <div class="form-group">
          <div class="password-input-wrapper">
            <input 
              :type="passwordFieldType"
              v-model="password"
              placeholder="Введите пароль"
              :class="{ 'is-invalid': passwordError || globalError }"
            >
            <button type="button" @click="togglePasswordVisibility('password')" class="toggle-visibility">
              <svg width="22" height="22" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                <path d="M1 12s4-8 11-8 11 8 11 8-4 8-11 8-11-8-11-8z"/>
                <circle cx="12" cy="12" r="3"/>
              </svg>
            </button>
          </div>
        </div>
        
        <div class="form-group">
          <div class="password-input-wrapper">
             <input 
              :type="confirmPasswordFieldType"
              v-model="passwordConfirm"
              placeholder="Повторите пароль"
              :class="{ 'is-invalid': passwordConfirmError || globalError }"
            >
            <button type="button" @click="togglePasswordVisibility('confirm')" class="toggle-visibility">
              <svg width="22" height="22" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                <path d="M1 12s4-8 11-8 11 8 11 8-4 8-11 8-11-8-11-8z"/>
                <circle cx="12" cy="12" r="3"/>
              </svg>
            </button>
          </div>
        </div>

        <p class="password-hint">
          Пароль должен содержать не менее 8 символов, включая латинские буквы (a-z, A-Z), как минимум одну заглавную букву и одну цифру
        </p>
        
        <button 
          type="submit" 
          class="submit-button" 
          :class="{ 'btn-error': globalError || (!isPasswordValid && submitted) }"
        >
          Далее
        </button>
      </form>

      <footer class="form-footer">
        <p>По всем вопросам можете обращаться:</p>
        <a href="mailto:adminexample@gmail.com">adminexample@gmail.com</a>
      </footer>
    </div>
  </div>
</template>

<script setup>
import { ref, computed } from 'vue';

const password = ref('');
const passwordConfirm = ref('');
const passwordFieldType = ref('password');
const confirmPasswordFieldType = ref('password');

const globalError = ref('');
const submitted = ref(false);

const passwordError = computed(() => submitted.value && !isPasswordValid.value);
const passwordConfirmError = computed(() => submitted.value && password.value !== passwordConfirm.value);

const togglePasswordVisibility = (field) => {
  if (field === 'password') {
    passwordFieldType.value = passwordFieldType.value === 'password' ? 'text' : 'password';
  } else {
    confirmPasswordFieldType.value = confirmPasswordFieldType.value === 'password' ? 'text' : 'password';
  }
};

const passwordStrength = computed(() => {
  if (!password.value) return { text: '', style: { width: '0%' } };
  
  let score = 0;
  if (password.value.length >= 8) score++;
  if (/[A-Z]/.test(password.value)) score++;
  if (/[0-9]/.test(password.value)) score++;
  
  // Цвета из вашей палитры ($dangerous для слабого, $light-gray для фона)
  if (score <= 1) {
    return { text: 'Слабый', style: { width: '33%', backgroundColor: '#e63946' } };
  } else if (score === 2) {
    return { text: 'Нормальный', style: { width: '66%', backgroundColor: '#a8a8a8' } };
  }
  return { text: 'Сильный', style: { width: '100%', backgroundColor: '#1f7a5a' } };
});

const isPasswordValid = computed(() => {
    return password.value.length >= 8 && /[A-Z]/.test(password.value) && /[0-9]/.test(password.value);
});

const handleSubmit = () => {
  submitted.value = true;
  globalError.value = '';

  if (!isPasswordValid.value) return;

  if (password.value !== passwordConfirm.value) {
    globalError.value = 'Пароли не совпадают, попробуйте снова';
    return;
  }
  
  console.log('Пароль изменен');
};
</script>

<style scoped lang="scss">
.form-wrapper {
  display: flex;
  justify-content: center;
  align-items: center;
  min-height: 100vh;
  background-color: variables.$dark-white;
  padding: 20px;
}

.form-container {
  background-color: variables.$white;
  border-radius: functions.radius('xxl');
  padding: 40px 32px;
  width: 100%;
  max-width: 440px;
  text-align: center;
}

.form-header h1 {
  font-size: 32px;
  font-weight: 700;
  color: variables.$black;
  margin-bottom: 32px;
  line-height: 1.1;
}

.global-error-message {
  color: variables.$dangerous;
  font-size: 14px;
  margin-bottom: 20px;
  font-weight: 500;
}

.strength-indicator {
  text-align: left;
  margin-bottom: 24px;

  .strength-label {
    font-size: 14px;
    color: variables.$gray;
    margin-bottom: 8px;
  }

  .strength-visual {
    display: flex;
    flex-direction: column;
    gap: 12px;
  }

  .strength-bar-bg {
    height: 6px;
    background-color: variables.$dark-white;
    border-radius: 10px;
    overflow: hidden;
  }

  .strength-bar-fill {
    height: 100%;
    transition: width 0.3s ease, background-color 0.3s ease;
  }

  .strength-text {
    font-size: 14px;
    color: variables.$gray;
    min-width: 80px;
    text-align: right;
  }
}

.form-group {
  margin-bottom: 16px;
}

.password-input-wrapper {
  position: relative;

  input {
    width: 100%;
    padding: 16px 50px 16px 20px;
    background-color: variables.$dark-white;
    border: 1px solid transparent;
    border-radius: functions.radius('m');
    font-size: 16px;
    color: variables.$black;
    box-sizing: border-box;
    transition: all 0.2s;

    &::placeholder {
      color: variables.$gray;
      letter-spacing: normal;
    }

    &:focus {
      outline: none;
      background-color: variables.$white;
      border-color: variables.$light-main-color;
    }

    &.is-invalid {
      border-color: variables.$dangerous;
      background-color: variables.$white;
    }
  }
}

.toggle-visibility {
  position: absolute;
  right: 16px;
  top: 50%;
  transform: translateY(-50%);
  background: none;
  border: none;
  cursor: pointer;
  color: variables.$gray;
  display: flex;
  align-items: center;
  padding: 0;

  &:hover { color: variables.$black; }
}

.password-hint {
  font-size: 12px;
  color: variables.$gray;
  line-height: 1.4;
  margin: 20px 0 32px;
  text-align: center;
}

.submit-button {
  width: 100%;
  padding: 18px;
  background-color: variables.$main-color;
  color: variables.$white;
  border: none;
  border-radius: functions.radius('m');
  font-size: 18px;
  font-weight: 600;
  cursor: pointer;
  transition: background-color 0.2s;

  &.btn-error {
    background-color: variables.$light-main-color;
  }
}

.form-footer {
  margin-top: 32px;
  font-size: 13px;
  color: variables.$gray;
  
  p { margin: 0; }
  a { 
    color: variables.$gray; 
    text-decoration: none;
    &:hover { text-decoration: underline; }
  }
}
</style>