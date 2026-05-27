<template>
  <div class="auth-container">
    <div class="auth-card">
      <header class="header">
        <button class="back-btn" @click="router.back()">
          <svg width="10" height="16" viewBox="0 0 10 16" fill="none">
            <path d="M8.5 15L1.5 8L8.5 1" stroke="#8C93A1" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"/>
          </svg>
        </button>
        <h1 class="title">Подтвердите<br>вашу почту</h1>
      </header>

      <div class="message-block">
        <p v-if="!hasError" class="info-text">
          Мы отправили код подтверждения<br>на вашу почту
        </p>
        <p v-else class="error-message">
          Неверный код подтверждения<br>
          Попробуйте еще раз или запросите код повторно
        </p>
      </div>

      <div class="otp-inputs">
        <input
          v-for="(digit, index) in 6"
          :key="index"
          type="text"
          inputmode="numeric"
          maxlength="1"
          v-model="code[index]"
          class="otp-field"
          :class="{ 
            'field-error': hasError, 
            'field-active': code[index] !== '' && !hasError 
          }"
          @input="handleInput($event, index)"
          @keydown.delete="handleDelete($event, index)"
          ref="inputs"
        />
      </div>

      <div class="timer-block">
        <p v-if="timer > 0" class="timer-text">
          Вы можете запросить код повторно через {{ timer }} секунд
        </p>
        <button v-else type="button" class="resend-btn" @click="resendCode">
          Запросить код повторно
        </button>
      </div>

      <button 
        class="submit-btn" 
        :class="{ 'btn-error-state': hasError }"
        :disabled="!isCodeComplete"
        @click="verifyCode"
      >
        Далее
      </button>

      <footer class="footer">
        <p>По всем вопросам можете обращаться:</p>
        <a href="mailto:adminexample@gmail.com">adminexample@gmail.com</a>
      </footer>
    </div>
  </div>
</template>

<script setup>
import { ref, computed, onMounted } from 'vue';
import { useRouter } from 'vue-router';
import { useAuthStore } from '@/stores/auth';

const props = defineProps({
  mode: { type: String, default: 'registration' }
});

const router = useRouter();
const code = ref(['', '', '', '', '', '']);
const inputs = ref([]);
const timer = ref(119); // Значение с макета
const hasError = ref(false); 

const isCodeComplete = computed(() => code.value.every(d => d !== ''));

onMounted(() => {
  startTimer();
});

function startTimer() {
  const countdown = setInterval(() => {
    if (timer.value > 0) timer.value--;
    else clearInterval(countdown);
  }, 1000);
}

const handleInput = (e, index) => {
  const val = e.target.value;
  hasError.value = false; // Сбрасываем ошибку при начале ввода
  if (!/^\d*$/.test(val)) {
    code.value[index] = '';
    return;
  }
  if (val && index < 5) {
    inputs.value[index + 1].focus();
  }
};

const handleDelete = (e, index) => {
  if (!code.value[index] && index > 0) {
    inputs.value[index - 1].focus();
  }
};

const verifyCode = () => {
  const enteredCode = code.value.join('');
  const authStore = useAuthStore();
  const userId = authStore.currentUserId;
  authStore.verifyEmail(enteredCode)
    .then(() => {
      router.push(props.mode === 'registration' ? '/final-registration' : '/set-new-password');
    })
    .catch((error) => {
      const errorMessage = error.response?.data?.error || 'Неверный или истекший код подтверждения';
      hasError.value = true;
      ElMessage.error(errorMessage);
    });
};

const resendCode = () => {
  timer.value = 119;
  hasError.value = false;
  code.value = ['', '', '', '', '', ''];
  startTimer();
};
</script>

<style scoped lang="scss">
.auth-container {
  display: flex;
  justify-content: center;
  align-items: center;
  min-height: 100vh;
  background-color: variables.$dark-white;
}

.auth-card {
  width: 100%;
  max-width: 420px;
  padding: 20px;
  text-align: center;
  background-color: variables.$white;
  border-radius: functions.radius('xxl');
}

.header {
  position: relative;
  margin-bottom: 30px;
}

.back-btn {
  position: absolute;
  left: 0;
  top: 10px;
  background: none;
  border: none;
  cursor: pointer;
  padding: 5px;
}

.title {
  font-size: 32px;
  font-weight: 700;
  color: variables.$black;
  line-height: 1.2;
  margin: 0;
}

.error-message {
  color: variables.$dangerous;
  font-size: 14px;
  line-height: 1.5;
  margin-bottom: 35px;
}

.otp-inputs {
  display: flex;
  justify-content: center;
  gap: 10px;
  margin-bottom: 30px;
}

.otp-field {
  width: 45px;
  height: 55px;
  border: 1px solid variables.$light-main-color;
  border-radius: functions.radius('s');
  text-align: center;
  font-size: 24px;
  font-weight: 600;
  color: variables.$light-main-color;
  background-color: variables.$white;
  outline: none;
  transition: all 0.2s;
}

.otp-field:focus {
  border-color: variables.$main-color;
  background-color: variables.$dark-white;
  color: variables.$main-color;
}

.field-error {
  border-color: variables.$dangerous;
  color: variables.$dangerous;
}

.timer-block {
  background-color: variables.$dark-white;
  border-radius: functions.radius('s');
  padding: 16px;
  margin-bottom: 25px;
}

.timer-text {
  color: variables.$gray;
  font-size: 15px;
  margin: 0;
}

.resend-btn {
  background: none;
  background-color: variables.$dark-white;
  border: none;
  color: variables.$gray;
  font-size: 15px;
  cursor: pointer;
  font-weight: 500;
}

.resend-btn:hover {
    color: variables.$main-color;
    transition: color 0.2s ease;
}

.submit-btn {
  width: 100%;
  padding: 16px;
  background-color: variables.$main-color; 
  color: variables.$white;
  border: none;
  border-radius: functions.radius('s');
  font-size: 18px;
  font-weight: 500;
  cursor: pointer;
  transition: opacity 0.2s;
}

.submit-btn:disabled {
  opacity: 0.7;
  cursor: not-allowed;
}

.footer {
  margin-top: 30px;
  font-size: 13px;
  color: variables.$gray;
  line-height: 1.6;
}

.footer a {
  color: variables.$gray;
  text-decoration: none;
}
</style>