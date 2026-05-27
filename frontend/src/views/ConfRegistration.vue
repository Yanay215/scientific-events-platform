<template>
  <div class="reg-container">
    <div class="reg-card">
      
      <header class="reg-header">
        <h1 class="reg-title">Регистрация<br>на конференцию</h1>
        <a href="#" class="conf-link">Название конференции</a>
      </header>

      <form @submit.prevent="handleNext" class="reg-form">
        
        <div class="form-group">
          <label class="form-label">
            Введите УДК: <span class="required">*</span>
          </label>
          <span class="form-sublabel">Справочник по УДК</span>
          <input 
            type="text" 
            v-model="udkValue" 
            class="form-input" 
            placeholder="00.00"
          >
        </div>

        <div class="form-group">
          <label class="form-label">
            Выберите секцию конференции: <span class="required">*</span>
          </label>
          <div class="select-wrapper">
            <select v-model="selectedSection" class="form-input form-select">
              <option value="" disabled selected>Выберите секцию</option>
              <option value="it">Информационные технологии</option>
              <option value="math">Математическое моделирование</option>
              <option value="phys">Прикладная физика</option>
            </select>
          </div>
        </div>

        <button type="submit" class="btn-submit">Далее</button>
      </form>

      <footer class="reg-footer">
        <p>По всем вопросам можете обращаться:</p>
        <a href="mailto:adminexample@gmail.com">adminexample@gmail.com</a>
      </footer>
      
    </div>
  </div>
</template>

<script setup>
import { ref } from 'vue';

const udkValue = ref('');
const selectedSection = ref('');

const handleNext = () => {
  console.log('Данные секции:', {
    udk: udkValue.value,
    section: selectedSection.value
  });
};
</script>

<style scoped>
.reg-container {
  display: flex;
  justify-content: center;
  align-items: center;
  min-height: 100vh;
  background-color: #ffffff;
  font-family: 'Inter', sans-serif;
  padding: 20px;
}

.reg-card {
  width: 100%;
  max-width: 420px;
  text-align: center;
}

/* Заголовки */
.reg-header {
  margin-bottom: 32px;
}

.reg-title {
  font-size: 36px;
  font-weight: 700;
  color: #333;
  line-height: 1.1;
  margin-bottom: 24px;
}

.conf-link {
  color: #003182;
  text-decoration: none;
  font-size: 16px;
  font-weight: 500;
}

.conf-link:hover {
  text-decoration: underline;
}

/* Форма */
.reg-form {
  display: flex;
  flex-direction: column;
  gap: 24px;
}

.form-group {
  text-align: left;
  display: flex;
  flex-direction: column;
}

.form-label {
  font-size: 18px;
  color: #333;
  font-weight: 500;
  margin-bottom: 4px;
}

.form-sublabel {
  font-size: 13px;
  color: #8C93A1;
  margin-bottom: 12px;
}

.required {
  color: #E53935;
}

.form-input {
  width: 100%;
  padding: 18px 20px;
  background-color: #F4F5F8;
  border: none;
  border-radius: 16px;
  font-size: 16px;
  color: #333;
  outline: none;
  box-sizing: border-box;
  transition: background-color 0.2s;
}

.form-input::placeholder {
  color: #8C93A1;
}

.form-input:focus {
  background-color: #EDEFF3;
}

/* Кастомный селект */
.select-wrapper {
  position: relative;
}

.form-select {
  appearance: none;
  cursor: pointer;
}

.select-wrapper::after {
  content: "";
  position: absolute;
  right: 20px;
  top: 50%;
  transform: translateY(-50%);
  width: 16px;
  height: 10px;
  background-image: url("data:image/svg+xml,%3Csvg width='14' height='9' viewBox='0 0 14 9' fill='none' xmlns='http://www.w3.org/2000/svg'%3E%3Cpath d='M1 1L7 7L13 1' stroke='%238C93A1' stroke-width='2' stroke-linecap='round'/%3E%3C/svg%3E");
  background-repeat: no-repeat;
  pointer-events: none;
}

/* Кнопка */
.btn-submit {
  width: 100%;
  padding: 18px;
  background-color: #003182;
  color: #fff;
  border: none;
  border-radius: 16px;
  font-size: 18px;
  font-weight: 600;
  cursor: pointer;
  margin-top: 8px;
  transition: background-color 0.2s;
}

.btn-submit:hover {
  background-color: #002561;
}

/* Футер */
.reg-footer {
  margin-top: 32px;
  font-size: 13px;
  color: #8C93A1;
  line-height: 1.6;
}

.reg-footer a {
  color: #8C93A1;
  text-decoration: none;
}

.reg-footer a:hover {
  text-decoration: underline;
}
</style>