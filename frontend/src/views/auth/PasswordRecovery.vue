<template>
  <div class="form-wrapper">
    <div class="form-container">
      <header class="form-header">
        <h1>Восстановление пароля</h1>
        <p>
          Введите вашу электронную почту, чтобы получить код подтверждения и восстановить доступ к аккаунту
        </p>
      </header>

      <form @submit.prevent="handleSubmit" novalidate>
        <div class="form-group">
          <input 
            type="email" 
            v-model="email" 
            placeholder="Почта" 
            :class="{ 'has-error': error }"
            aria-label="Электронная почта"
          >
          <span v-if="error" class="error-message">{{ error }}</span>
        </div>
        
        <div class="button-group">
          <button type="submit" class="submit-button">Отправить код</button>
          <button type="button" class="cancel-button" @click="handleCancel">Отмена</button>
        </div>
      </form>

      <footer class="form-footer">
        <p>По всем вопросам можете обращаться:<br>adminexample@gmail.com</p>
      </footer>
    </div>
  </div>
</template>

<script setup>
import { ref } from 'vue';
import { useRouter } from 'vue-router';

const router = useRouter();
const email = ref('');
const error = ref('');

const validateEmail = (val) => {
  if (!val) {
    return 'Поле не может быть пустым';
  }
  const re = /^\S+@\S+\.\S+$/;
  if (!re.test(val)) {
    return 'Некорректный формат email';
  }
  return '';
};

const clearError = () => {
  error.value = '';
};

const handleSubmit = () => {
  error.value = validateEmail(email.value);

  if (!error.value) {
    console.log('Отправка кода для:', email.value);
    router.push('/reset-password');
  }
};

const handleCancel = () => {
  email.value = '';
  error.value = '';
  router.back();
};
</script>

<style scoped lang="scss">
.form-wrapper {
  width: 100%;
  padding: 20px;
  display: flex;
  justify-content: center;
}

.form-container {
  background-color: variables.$white;
  border-radius: functions.radius('xxl');
  padding: 32px;
  width: 100%;
  max-width: 420px;
  box-shadow: 0 10px 25px -5px rgba(0, 0, 0, 0.1), 0 10px 10px -5px rgba(0, 0, 0, 0.04);
  text-align: center;
}

.form-header h1 {
  font-size: 32px;
  font-weight: 700;
  margin: 0 0 16px 0;
  line-height: 1.2;
}

.form-header p {
  font-size: 16px;
  color: variables.$black;
  line-height: 1.5;
  margin: 0 0 32px 0;
}

.form-group {
  margin-bottom: 24px;
}

input[type="email"] {
  width: 100%;
  padding: 16px;
  background-color: variables.$dark-white;
  border: 1px solid transparent;
  border-radius: functions.radius('m');
  font-size: 16px;
  box-sizing: border-box;
  text-align: left;
  transition: border-color 0.2s, box-shadow 0.2s;
}

input[type="email"].has-error {
  border-color: variables.$dangerous;
}
input[type="email"].has-error:focus {
  box-shadow: 0 0 0 3px rgba(229, 62, 62, 0.1);
}

.error-message {
  color: variables.$dangerous;
  font-size: 14px;
  margin-top: 8px;
  display: block;
  text-align: left;
}

.button-group {
  display: flex;
  flex-direction: column;
  gap: 12px;
}

.submit-button,
.cancel-button {
  width: 100%;
  padding: 16px;
  border-radius: functions.radius('m');
  font-size: 18px;
  font-weight: 600;
  cursor: pointer;
  border: none;
  transition: all 0.3s;
}

.submit-button {
  background-color: variables.$main-color;
  color: variables.$white;
}

.cancel-button {
  background-color: variables.$dark-white;
  color: variables.$black;
  font-weight: 500;
}

.form-footer {
  margin-top: 32px;
  font-size: 13px;
  color: variables.$gray;
  line-height: 1.5;
}

.form-footer p {
  margin: 0;
}
</style>