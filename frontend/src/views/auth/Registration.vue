<template>
  <div class="registration-container">
    <div class="registration-card">
      
      <header class="header">
        <button class="back-btn" @click="$router.back()" aria-label="Назад">
          <svg width="10" height="16" viewBox="0 0 10 16" fill="none" xmlns="http://www.w3.org/2000/svg">
            <path d="M8.5 15L1.5 8L8.5 1" stroke="#8C93A1" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"/>
          </svg>
        </button>
        <h1 class="title">Регистрация</h1>
      </header>

      <form @submit.prevent="handleSubmit" class="form">
        
        <div class="field-group">
          <input
            v-model="email"
            type="email"
            class="input"
            :class="{ 'input-error': emailError }"
            placeholder="Введите почту"
            @blur="validateEmail"
            @input="emailError = false"
          />
          <p v-if="emailError" class="error-text">Неккоректный формат email</p>
        </div>

        <div class="password-strength-block">
          <div class="strength-header">
            <span class="strength-label">Сложность пароля:</span>
            <el-tooltip placement="top" effect="light">
              <template #content>
                <div class="pwd-req-list">
                  <div :class="{ ok: hasMinLen }">{{ hasMinLen ? '✓' : '•' }} не менее 8 символов</div>
                  <div :class="{ ok: hasUpper }">{{ hasUpper ? '✓' : '•' }} заглавная латинская буква (A-Z)</div>
                  <div :class="{ ok: hasLower }">{{ hasLower ? '✓' : '•' }} строчная латинская буква (a-z)</div>
                  <div :class="{ ok: hasDigit }">{{ hasDigit ? '✓' : '•' }} хотя бы одна цифра</div>
                </div>
              </template>
              <span class="pwd-req-icon" role="img" aria-label="Требования к паролю">?</span>
            </el-tooltip>
          </div>
          <div class="strength-bar-bg">
            <div 
              class="strength-bar-fill" 
              :style="{ width: strengthPercentage + '%', backgroundColor: strengthClass === 'strength-weak' ? '#E53935' : strengthClass === 'strength-medium' ? '#FFB300' : '#43A047' }"
            ></div>
          </div>
          <div class="strength-footer">
            <span class="strength-text" :style="{ color: strengthClass === 'strength-weak' ? '#E53935' : strengthClass === 'strength-medium' ? '#FFB300' : '#43A047' }">{{ strengthLabel }}</span>
          </div>
        </div>

        <div class="passwords-group">
          <div class="field-wrap input-icon-wrap">
            <input
              v-model="password"
              :type="showPassword ? 'text' : 'password'"
              class="input"
              :class="{ 'input-error': passwordError }"
              placeholder="Введите пароль"
              @input="handlePasswordInput"
            />
            <button type="button" class="eye-btn" @click="showPassword = !showPassword">
              <svg width="20" height="20" viewBox="0 0 24 24" fill="none" :stroke="passwordError ? '#E53935' : '#8C93A1'" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                <path v-if="!showPassword" d="M1 12s4-8 11-8 11 8 11 8-4 8-11 8-11-8-11-8z"></path>
                <circle v-if="!showPassword" cx="12" cy="12" r="3"></circle>
                <path v-if="showPassword" d="M17.94 17.94A10.07 10.07 0 0 1 12 20c-7 0-11-8-11-8a18.45 18.45 0 0 1 5.06-5.94M9.9 4.24A9.12 9.12 0 0 1 12 4c7 0 11 8 11 8a18.5 18.5 0 0 1-2.16 3.19m-6.72-1.07a3 3 0 1 1-4.24-4.24"></path>
                <line v-if="showPassword" x1="1" y1="1" x2="23" y2="23"></line>
              </svg>
            </button>
          </div>

          <div class="field-wrap input-icon-wrap">
            <input
              v-model="repeatPassword"
              :type="showRepeatPassword ? 'text' : 'password'"
              class="input"
              :class="{ 'input-error': passwordError }"
              placeholder="Повторите пароль"
              @blur="validatePasswords"
              @input="handlePasswordInput"
            />
            <button type="button" class="eye-btn" @click="showRepeatPassword = !showRepeatPassword">
              <svg width="20" height="20" viewBox="0 0 24 24" fill="none" :stroke="passwordError ? '#E53935' : '#8C93A1'" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                <path v-if="!showRepeatPassword" d="M1 12s4-8 11-8 11 8 11 8-4 8-11 8-11-8-11-8z"></path>
                <circle v-if="!showRepeatPassword" cx="12" cy="12" r="3"></circle>
                <path v-if="showRepeatPassword" d="M17.94 17.94A10.07 10.07 0 0 1 12 20c-7 0-11-8-11-8a18.45 18.45 0 0 1 5.06-5.94M9.9 4.24A9.12 9.12 0 0 1 12 4c7 0 11 8 11 8a18.5 18.5 0 0 1-2.16 3.19m-6.72-1.07a3 3 0 1 1-4.24-4.24"></path>
                <line v-if="showRepeatPassword" x1="1" y1="1" x2="23" y2="23"></line>
              </svg>
            </button>
          </div>
          <p v-if="passwordError" class="error-text">Пароли не совпадают, попробуйте снова</p>
        </div>

        <p class="password-hint">
          Пароль должен содержать не менее 8 символов, включая латинские буквы (a-z, A-Z), как минимум одну заглавную букву и одну цифру
        </p>

        <button 
          type="submit" 
          class="submit-btn" 
          :disabled="!isFormValid"
        >
          Далее
        </button>
      </form>

      <div class="footer">
        <p>По всем вопросам можете обращаться:</p>
        <a href="mailto:adminexample@gmail.com">adminexample@gmail.com</a>
      </div>

    </div>
  </div>
</template>

<script setup>
import { ref, computed } from 'vue'
import { useRouter } from 'vue-router'
import { useAuthStore } from '@/stores/auth'
import { ElMessage } from 'element-plus'

const email = ref('')
const password = ref('')
const repeatPassword = ref('')

const showPassword = ref(false)
const showRepeatPassword = ref(false)

const emailError = ref(false)
const passwordError = ref(false)

const router = useRouter()

const hasMinLen = computed(() => password.value.length >= 8)
const hasUpper = computed(() => /[A-Z]/.test(password.value))
const hasLower = computed(() => /[a-z]/.test(password.value))
const hasDigit = computed(() => /\d/.test(password.value))
const passwordMeetsRequirements = computed(() =>
  hasMinLen.value && hasUpper.value && hasLower.value && hasDigit.value
)

const validateEmail = () => {
  const re = /^[^\s@]+@[^\s@]+\.[^\s@]+$/
  emailError.value = !!(email.value && !re.test(email.value))
}

const validatePasswords = () => {
  passwordError.value = !!(password.value && repeatPassword.value && password.value !== repeatPassword.value)
}

const handlePasswordInput = () => {
  passwordError.value = false
}

const isFormValid = computed(() => {
  return email.value 
    && password.value 
    && repeatPassword.value 
    && !emailError.value 
    && !passwordError.value
    && passwordMeetsRequirements.value
})

const strengthPercentage = computed(() => {
  if (!password.value) return 0
  let score = 0
  if (password.value.length >= 8) score += 33
  if (/[a-z]/.test(password.value) && /[A-Z]/.test(password.value)) score += 33
  if (/\d/.test(password.value) && /[^a-zA-Z0-9]/.test(password.value)) score += 34
  return Math.min(100, score) 
})

const strengthLabel = computed(() => {
  if (!password.value) return ''
  if (strengthPercentage.value <= 33) return 'Слабый'
  if (strengthPercentage.value <= 66) return 'Средний'
  return 'Надежный'
})

const strengthClass = computed(() => {
  if (!password.value) return '' 
  const percentage = strengthPercentage.value
  if (percentage <= 33) return 'strength-weak'
  if (percentage <= 66) return 'strength-medium'
  return 'strength-strong'
})

const handleSubmit = () => {
  validateEmail()
  validatePasswords()
  
  if (isFormValid.value) {
    useAuthStore().register(email.value, password.value)
      .then(() => {
        router.push('/verify-email')
      })
      .catch((error) => {
        const errorMessage = error.response?.data?.error || 'Ошибка при регистрации';
        ElMessage.error(errorMessage);
      })
  }
}
</script>

<style scoped lang="scss">
.registration-container {
  display: flex;
  justify-content: center;
  align-items: center;
  min-height: 100vh;
  background-color: variables.$dark-white;
}

.registration-card {
  width: 100%;
  max-width: 400px;
  padding: 32px 24px;
  background-color: variables.$white;
  box-sizing: border-box;
  border-radius: functions.radius('xl');
}

.header {
  display: flex;
  align-items: center;
  margin-bottom: 32px;
  position: relative;
}

.back-btn {
  background: none;
  border: none;
  cursor: pointer;
  padding: 8px;
  position: absolute;
  left: -8px;
  display: flex;
  align-items: center;
  justify-content: center;
  transition: opacity 0.2s;
}

.title {
  flex: 1;
  text-align: center;
  font-size: 32px;
  font-weight: 700;
  color: variables.$black;
  margin: 0;
}

.form {
  display: flex;
  flex-direction: column;
  gap: 20px;
}

.field-group {
  display: flex;
  flex-direction: column;
}

.field-wrap {
  width: 100%;
}

.passwords-group {
  display: flex;
  flex-direction: column;
  gap: 16px;
}

.input {
  width: 100%;
  padding: 18px 20px;
  background-color: variables.$dark-white;
  border: 1px solid transparent;
  border-radius: functions.radius('m');
  font-size: 16px;
  color: variables.$black;
  outline: none;
  box-sizing: border-box;
  transition: all 0.2s;
}

.input::placeholder {
  color: variables.$gray;
}

.input-error {
  border-color: variables.$dangerous;
}

.error-text {
  color: variables.$dangerous;
  font-size: 13px;
  margin: 8px 0 0 8px;
}

.input-icon-wrap {
  position: relative;
  display: flex;
  align-items: center;
}

.input-icon-wrap .input {
  padding-right: 48px;
}

.eye-btn {
  position: absolute;
  right: 16px;
  background: none;
  border: none;
  cursor: pointer;
  display: flex;
  align-items: center;
  justify-content: center;
  padding: 4px;
}

.password-strength-block {
  display: flex;
  flex-direction: column;
  gap: 8px;
  margin-top: -4px;
}

.strength-header {
  font-size: 15px;
  color: variables.$gray;
  display: flex;
  align-items: center;
  gap: 6px;
}

.pwd-req-icon {
  display: inline-flex;
  align-items: center;
  justify-content: center;
  width: 16px;
  height: 16px;
  border-radius: 50%;
  background-color: variables.$main-color;
  color: variables.$white;
  font-size: 11px;
  font-weight: 700;
  cursor: help;
  user-select: none;
}

.pwd-req-list {
  font-size: 13px;
  line-height: 1.7;
  color: variables.$gray;

  .ok {
    color: #43A047;
  }
}

.strength-bar-bg {
  width: 100%;
  height: 6px;
  background-color: variables.$dark-white;
  border-radius: functions.radius('xxs');
  overflow: hidden;
}

.strength-bar-fill {
  height: 100%;
  border-radius: functions.radius('xxs');
  transition: width 0.3s ease, background-color 0.3s ease;
}

.strength-footer {
  text-align: right;
  min-height: 16px;
}

.strength-text {
  font-size: 14px;
}

.password-hint {
  font-size: 12px;
  line-height: 1.5;
  color: variables.$gray;
  text-align: center;
  margin: 0;
  padding: 0 10px;
}

.submit-btn {
  width: 100%;
  padding: 18px;
  background-color: variables.$main-color;
  color: variables.$white;
  font-size: 18px;
  font-weight: 500;
  border: none;
  border-radius: functions.radius('m');
  cursor: pointer;
  transition: background-color 0.2s, opacity 0.2s;
  margin-top: 12px;
}

.submit-btn:disabled {
  background-color: variables.$light-main-color;
  cursor: not-allowed;
}

.footer {
  margin-top: 24px;
  text-align: center;
  font-size: 13px;
  color: variables.$gray;
  line-height: 1.6;
}

.footer p {
  margin: 0;
}

.footer a {
  color: variables.$gray;
  text-decoration: none;
}

.footer a:hover {
  text-decoration: underline;
}
</style>