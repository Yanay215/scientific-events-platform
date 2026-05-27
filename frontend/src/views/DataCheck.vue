<template>
  <div class="registration-container">
    <div class="registration-card">
      
      <header class="header">
        <h1 class="title">Проверьте ваши<br>личные данные</h1>
        <p class="subtitle">Название конференции</p>
      </header>

      <form @submit.prevent="handleSubmit" class="form">
        
        <div class="form-group">
          <label class="label">ФИО (на русском): <span class="required">*</span></label>
          <input type="text" class="input" placeholder="Волков Олег Олегович" v-model="form.fioRu">
        </div>

        <div class="form-group">
          <label class="label">ФИО (на английском): <span class="required">*</span></label>
          <input type="text" class="input" placeholder="Volkov Oleg Olegovich" v-model="form.fioEn">
        </div>

        <div class="form-group">
          <label class="label">Город (на русском): <span class="required">*</span></label>
          <div class="select-wrapper">
            <select class="input select" v-model="form.cityRu">
              <option value="" disabled selected>Выберите город</option>
              <option value="moscow">Москва</option>
              <option value="spb">Санкт-Петербург</option>
            </select>
          </div>
        </div>

        <div class="form-group">
          <label class="label">Город (на английском): <span class="required">*</span></label>
          <div class="select-wrapper">
            <select class="input select" v-model="form.cityEn">
              <option value="" disabled selected>Выберите курс/должность/степень</option>
              <option value="student">Student</option>
              <option value="professor">Professor</option>
            </select>
          </div>
        </div>

        <div class="form-group">
          <label class="label">Почта: <span class="required">*</span></label>
          <input type="email" class="input" placeholder="example_email@gmail.com" v-model="form.email">
        </div>

        <div class="form-group">
          <label class="label">Ваш номер телефона:</label>
          <input type="tel" class="input" placeholder="+7 (999) 999 - 99 - 99" v-model="form.phone">
        </div>

        <div class="form-group">
          <label class="label">ВУЗ (на английском): <span class="required">*</span></label>
          <input type="text" class="input" placeholder="Введите название вашего ВУЗа" v-model="form.universityEn">
        </div>

        <div class="form-group">
          <label class="label">Подразделение (на русском): <span class="required">*</span></label>
          <p class="hint">Структурное подразделение вашего ВУЗа</p>
          <input type="text" class="input" placeholder="Введите название подразделения" v-model="form.deptRu">
        </div>

        <div class="form-group">
          <label class="label">Подразделение (на английском): <span class="required">*</span></label>
          <p class="hint">Структурное подразделение вашего ВУЗа</p>
          <input type="text" class="input" placeholder="Введите название подразделения" v-model="form.deptEn">
        </div>

        <button type="submit" class="submit-btn">Далее</button>
      </form>

      <footer class="footer">
        <p>По всем вопросам можете обращаться:</p>
        <a href="mailto:adminexample@gmail.com">adminexample@gmail.com</a>
      </footer>
    </div>
  </div>
</template>

<script setup>
import { reactive } from 'vue';

const form = reactive({
  fioRu: '',
  fioEn: '',
  cityRu: '',
  cityEn: '',
  email: '',
  phone: '',
  universityEn: '',
  deptRu: '',
  deptEn: ''
});

const handleSubmit = () => {
  console.log('Данные формы:', form);
};
</script>

<style scoped>
.registration-container {
  display: flex;
  justify-content: center;
  padding: 40px 20px;
  background-color: #fff;
  font-family: 'Inter', sans-serif;
}

.registration-card {
  width: 100%;
  max-width: 440px;
}

.header {
  text-align: center;
  margin-bottom: 32px;
}

.title {
  font-size: 32px;
  font-weight: 700;
  color: #333;
  line-height: 1.2;
  margin-bottom: 24px;
}

.subtitle {
  color: #003182;
  font-size: 16px;
  font-weight: 500;
}

.form {
  display: flex;
  flex-direction: column;
  gap: 20px;
}

.form-group {
  display: flex;
  flex-direction: column;
}

.label {
  font-size: 16px;
  color: #333;
  margin-bottom: 12px;
  font-weight: 500;
}

.required {
  color: #E53935;
}

.hint {
  font-size: 12px;
  color: #8C93A1;
  margin-bottom: 8px;
  margin-top: -8px;
}

.input {
  width: 100%;
  padding: 16px 20px;
  background-color: #F4F5F8;
  border: none;
  border-radius: 14px;
  font-size: 15px;
  color: #333;
  outline: none;
  box-sizing: border-box;
}

.input::placeholder {
  color: #8C93A1;
}

/* Стилизация Select */
.select-wrapper {
  position: relative;
}

.select {
  appearance: none;
  cursor: pointer;
  color: #8C93A1;
}

.select-wrapper::after {
  content: "";
  position: absolute;
  right: 20px;
  top: 50%;
  transform: translateY(-50%);
  width: 12px;
  height: 8px;
  background-image: url("data:image/svg+xml,%3Csvg width='12' height='8' viewBox='0 0 12 8' fill='none' xmlns='http://www.w3.org/2000/svg'%3E%3Cpath d='M1 1L6 6L11 1' stroke='%238C93A1' stroke-width='2' stroke-linecap='round'/%3E%3C/svg%3E");
  background-repeat: no-repeat;
  pointer-events: none;
}

/* Кнопка */
.submit-btn {
  width: 100%;
  padding: 18px;
  background-color: #003182;
  color: #fff;
  border: none;
  border-radius: 14px;
  font-size: 18px;
  font-weight: 600;
  cursor: pointer;
  margin-top: 10px;
  transition: background 0.2s;
}

.submit-btn:hover {
  background-color: #002566;
}

/* Футер */
.footer {
  margin-top: 24px;
  text-align: center;
  font-size: 13px;
  color: #8C93A1;
  line-height: 1.6;
}

.footer a {
  color: #8C93A1;
  text-decoration: none;
}

.footer a:hover {
  text-decoration: underline;
}
</style>