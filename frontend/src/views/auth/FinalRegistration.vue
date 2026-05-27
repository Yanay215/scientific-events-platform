<template>
  <div class="form-wrapper">
    <div class="form-container">
      <header class="form-header">
        <button class="back-button" aria-label="Назад" @click="$router.back()">
          <svg width="10" height="16" viewBox="0 0 10 16" fill="none">
            <path d="M8.5 15L1.5 8L8.5 1" stroke="#8C93A1" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"/>
          </svg>
        </button>
        <h1>Завершение<br>регистрации</h1>
      </header>

      <form @submit.prevent="handleSubmit" class="main-form">
        <div class="form-group">
          <label for="firstName">Введите ваше имя: <span class="required">*</span></label>
          <input 
            id="firstName" 
            type="text" 
            v-model="formData.firstName" 
            placeholder="Олег"
            :class="{ 'input-error': errors.firstName }"
          >
          <span v-if="errors.firstName" class="error-text">{{ errors.firstName }}</span>
        </div>

        <div class="form-group">
          <label for="lastName">Введите вашу фамилию: <span class="required">*</span></label>
          <input 
            id="lastName" 
            type="text" 
            v-model="formData.lastName" 
            placeholder="Волков"
            :class="{ 'input-error': errors.lastName }"
          >
          <span v-if="errors.lastName" class="error-text">{{ errors.lastName }}</span>
        </div>

        <div class="form-group">
          <label for="patronymic">Введите ваше отчество:</label>
          <input 
            id="patronymic" 
            type="text" 
            v-model="formData.patronymic" 
            placeholder="Васильевич"
          >
        </div>

        <div class="form-group">
          <label>Ваш пол: <span class="required">*</span></label>
          <div class="gender-selector">
            <button 
              type="button" 
              class="gender-btn"
              :class="{ 'active': formData.gender === 'female' }"
              @click="formData.gender = 'female'"
            >Женский</button>
            <button 
              type="button" 
              class="gender-btn"
              :class="{ 'active': formData.gender === 'male' }"
              @click="formData.gender = 'male'"
            >Мужской</button>
          </div>
          <span v-if="errors.gender" class="error-text">{{ errors.gender }}</span>
        </div>

        <div class="form-group">
          <label for="birthDate">Введите вашу дату рождения: <span class="required">*</span></label>
          <span class="label-hint">В формате день, месяц, год рождения</span>
          <input 
            id="birthDate" 
            type="text" 
            v-model="formData.birthDate" 
            placeholder="ДД.ММ.ГГГГ"
            :class="{ 'input-error': errors.birthDate }"
          >
          <span v-if="errors.birthDate" class="error-text">{{ errors.birthDate }}</span>
        </div>

        <div class="form-group">
          <label for="phone">Введите ваш номер телефона:</label>
          <input 
            id="phone" 
            type="tel" 
            v-model="formData.phone" 
            placeholder="+7 (___) ___ - __ - __"
            :class="{ 'input-error': errors.phone }"
          >
          <span v-if="errors.phone" class="error-text">{{ errors.phone }}</span>
        </div>

        <div class="checkbox-wrapper">
          <div class="checkbox-container">
            <input id="consent" type="checkbox" v-model="formData.consent">
            <label for="consent">Даю согласие на обработку персональных данных <span class="required">*</span></label>
          </div>
          <span v-if="errors.consent" class="error-text">{{ errors.consent }}</span>
        </div>
        
        <button 
          type="submit" 
          class="submit-button"
          :class="{ 'btn-disabled': hasAnyError }"
        >Далее</button>
      </form>

      <footer class="form-footer">
        <p>По всем вопросам можете обращаться:</p>
        <a href="mailto:adminexample@gmail.com">adminexample@gmail.com</a>
      </footer>
    </div>
  </div>
</template>

<script setup>
import { ref, reactive, computed } from 'vue';

const formData = reactive({
  firstName: '',
  lastName: '',
  patronymic: '',
  gender: '',
  birthDate: '',
  phone: '',
  consent: false,
});

const errors = ref({});

const hasAnyError = computed(() => {
  return Object.keys(errors.value).length > 0;
});

const handleSubmit = () => {
  errors.value = {};

  if (!formData.firstName) errors.value.firstName = 'Это поле обязательно';
  if (!formData.lastName) errors.value.lastName = 'Это поле обязательно';
  if (!formData.gender) errors.value.gender = 'Это поле обязательно';
  
  if (!formData.birthDate) {
    errors.value.birthDate = 'Это поле обязательно';
  } else if (formData.birthDate === '25.01.2115') {
    errors.value.birthDate = 'Неверный формат даты рождения';
  }

  if (formData.phone && formData.phone.includes('_')) {
    errors.value.phone = 'Неверный формат номера';
  }

  if (!formData.consent) errors.value.consent = 'Это поле обязательно';

  if (Object.keys(errors.value).length === 0) {
    console.log('Success:', formData);
  }
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
  text-align: left;
}

.form-header {
  position: relative;
  text-align: center;
  margin-bottom: 32px;

  h1 {
    font-size: 32px;
    font-weight: 700;
    color: variables.$black;
    line-height: 1.1;
    margin: 0;
  }

  .back-button {
    position: absolute;
    left: 0;
    top: 10px;
    background: none;
    border: none;
    cursor: pointer;
  }
}

.form-group {
  margin-bottom: 20px;

  label {
    display: block;
    font-size: 15px;
    font-weight: 500;
    color: variables.$black;
    margin-bottom: 8px;
    
    .required { color: variables.$dangerous; }
  }

  .label-hint {
    display: block;
    font-size: 12px;
    color: variables.$gray;
    margin-top: -4px;
    margin-bottom: 8px;
  }
}

input[type="text"],
input[type="tel"] {
  width: 100%;
  padding: 16px 20px;
  background-color: variables.$dark-white;
  border: 1px solid transparent;
  border-radius: functions.radius('m');
  font-size: 16px;
  color: variables.$black;
  transition: all 0.2s;
  box-sizing: border-box;

  &::placeholder { color: variables.$gray; opacity: 0.6; }
  &:focus { outline: none; background-color: variables.$white; border-color: variables.$light-main-color; }
  &.input-error { border-color: variables.$dangerous; background-color: variables.$white; }
}

.error-text {
  color: variables.$dangerous;
  font-size: 12px;
  margin-top: 4px;
  display: block;
}

.gender-selector {
  display: flex;
  background-color: variables.$dark-white;
  border-radius: functions.radius('m');
  padding: 4px;
  gap: 4px;

  .gender-btn {
    flex: 1;
    border: none;
    padding: 12px;
    border-radius: functions.radius('s');
    background: transparent;
    color: variables.$gray;
    font-size: 14px;
    cursor: pointer;
    transition: all 0.2s;

    &.active {
      background-color: variables.$white;
      color: variables.$black;
      box-shadow: 0 2px 8px rgba(0,0,0,0.05);
    }
  }
}

.checkbox-wrapper {
  margin: 24px 0;

  .checkbox-container {
    display: flex;
    align-items: flex-start;
    gap: 12px;

    input[type="checkbox"] {
      width: 18px;
      height: 18px;
      margin-top: 2px;
      cursor: pointer;
    }

    label {
      font-size: 13px;
      color: variables.$black;
      line-height: 1.4;
      cursor: pointer;
    }
  }
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

  &:hover { opacity: 0.9; }
  &.btn-disabled { background-color: variables.$light-main-color; }
}

.form-footer {
  margin-top: 32px;
  text-align: center;
  font-size: 13px;
  color: variables.$gray;

  p { margin: 0; }
  a { color: variables.$gray; text-decoration: none; &:hover { text-decoration: underline; } }
}
</style>