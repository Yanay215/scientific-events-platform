<template>
  <div class="form-wrapper">
    <div class="form-card">
      <form @submit.prevent="submitForm">
        <div v-if="currentStep === 1" class="form-step">
          <h1 class="title">Проверьте ваши<br>личные данные</h1>
          
          <div class="form-group">
            <label class="form-label">ФИО (на русском): <span class="required">*</span></label>
            <input type="text" v-model="formData.personal.fioRu" placeholder="Волков Олег Олегович">
          </div>
          <div class="form-group">
            <label class="form-label">ФИО (на английском): <span class="required">*</span></label>
            <input type="text" v-model="formData.personal.fioEn" placeholder="Volkov Oleg Olegovich">
          </div>
          <div class="form-group">
            <label class="form-label">Город (на русском): <span class="required">*</span></label>
            <div class="select-wrapper">
              <select v-model="formData.personal.cityRu">
                <option disabled value="">Выберите город</option>
                <option>Москва</option>
                <option>Санкт-Петербург</option>
              </select>
              <svg class="select-arrow" width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><polyline points="6 9 12 15 18 9"></polyline></svg>
            </div>
          </div>
          <div class="form-group">
            <label class="form-label">Город (на английском): <span class="required">*</span></label>
            <div class="select-wrapper">
               <select v-model="formData.personal.cityEn">
                <option disabled value="">Выберите город</option>
                <option>Moscow</option>
                <option>Saint Petersburg</option>
              </select>
              <svg class="select-arrow" width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><polyline points="6 9 12 15 18 9"></polyline></svg>
            </div>
          </div>
          <div class="form-group">
            <label class="form-label">Почта: <span class="required">*</span></label>
            <input type="email" v-model="formData.personal.email" placeholder="example_email@gmail.com">
          </div>
          <div class="form-group">
            <label class="form-label">Ваш номер телефона:</label>
            <input type="tel" v-model="formData.personal.phone" placeholder="+7 (999) 999 - 99 - 99">
          </div>
        </div>

        <div v-if="currentStep === 2" class="form-step">
          <h1 class="title">Сведения об<br>обучении/работе</h1>

          <div class="form-group">
            <label class="form-label">Статус (на русском): <span class="required">*</span></label>
            <div class="select-wrapper">
              <select v-model="formData.education.statusRu">
                <option disabled value="">Выберите курс/должность/степень</option>
                <option>Студент</option>
                <option>Аспирант</option>
                <option>Сотрудник</option>
              </select>
              <svg class="select-arrow" width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><polyline points="6 9 12 15 18 9"></polyline></svg>
            </div>
          </div>
          <div class="form-group">
            <label class="form-label">Статус (на английском): <span class="required">*</span></label>
            <div class="select-wrapper">
              <select v-model="formData.education.statusEn">
                <option disabled value="">Выберите курс/должность/степень</option>
                <option>Student</option>
                <option>Postgraduate</option>
                <option>Employee</option>
              </select>
              <svg class="select-arrow" width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><polyline points="6 9 12 15 18 9"></polyline></svg>
            </div>
          </div>
          <div class="form-group">
            <label class="form-label">ВУЗ (на русском): <span class="required">*</span></label>
            <input type="text" v-model="formData.education.universityRu" placeholder="Введите название вашего ВУЗа">
          </div>
          <div class="form-group">
            <label class="form-label">ВУЗ (на английском): <span class="required">*</span></label>
            <input type="text" v-model="formData.education.universityEn" placeholder="Введите название вашего ВУЗа">
          </div>
          <div class="form-group">
            <label class="form-label">Подразделение (на русском): <span class="required">*</span></label>
            <p class="hint">Структурное подразделение вашего ВУЗа</p>
            <input type="text" v-model="formData.education.departmentRu" placeholder="Введите название подразделения">
          </div>
          <div class="form-group">
            <label class="form-label">Подразделение (на английском): <span class="required">*</span></label>
            <p class="hint">Структурное подразделение вашего ВУЗа</p>
            <input type="text" v-model="formData.education.departmentEn" placeholder="Введите название подразделения">
          </div>
        </div>

        <div v-if="currentStep === 3" class="form-step">
          <h1 class="title">Информация о работе</h1>
          
          <div class="form-group">
            <label class="form-label">Названия тезисов (на русском): <span class="required">*</span></label>
            <input type="text" v-model="formData.thesis.titleRu" placeholder="Введите название">
          </div>
          <div class="form-group">
            <label class="form-label">Названия тезисов (на английском): <span class="required">*</span></label>
            <input type="text" v-model="formData.thesis.titleEn" placeholder="Enter title">
          </div>
           <div class="form-group">
            <label class="form-label">Соавторы (на русском):</label>
            <p class="hint">Для каждого соавтора указать: ФИО, курс/должность/степень, подразделение, вуз, город и email</p>
            <textarea v-model="formData.thesis.coauthorsRu" rows="4" placeholder="Введите информацию"></textarea>
          </div>
           <div class="form-group">
            <label class="form-label">Соавторы (на английском):</label>
            <p class="hint">Указать: Full Name, status, department, university, city and email</p>
            <textarea v-model="formData.thesis.coauthorsEn" rows="4" placeholder="Enter info"></textarea>
          </div>
           <div class="form-group">
            <label class="form-label">Аннотация (на русском): <span class="required">*</span></label>
            <textarea v-model="formData.thesis.annotationRu" rows="4" placeholder="Краткое содержание"></textarea>
          </div>
          <div class="form-group">
            <label class="form-label">Аннотация (на английском): <span class="required">*</span></label>
            <textarea v-model="formData.thesis.annotationEn" rows="4" placeholder="Abstract content"></textarea>
          </div>
          <div class="form-group">
            <label class="form-label">Текст тезисов: <span class="required">*</span></label>
            <p class="hint">Допускается doc. Не более 5МБ</p>
            <div class="file-upload-area">
                <input type="file" id="file-upload" @change="handleFileUpload" class="file-input" accept=".doc,.docx">
                <label for="file-upload" class="file-label">
                    <span v-if="!formData.thesis.file" class="plus-icon">+</span>
                    <span v-else class="file-name">{{ formData.thesis.file.name }}</span>
                </label>
            </div>
          </div>
        </div>

        <div class="navigation-buttons">
            <button v-if="currentStep === 3" type="submit" class="btn-primary">Отправить</button>
            <button v-if="currentStep < 3" type="button" @click="nextStep" class="btn-primary">Далее</button>
            <button v-if="currentStep > 1" type="button" @click="prevStep" class="btn-secondary">Назад</button>
        </div>
      </form>

      <footer class="form-footer">
        <p>По всем вопросам можете обращаться:<br><a href="mailto:adminexample@gmail.com">adminexample@gmail.com</a></p>
      </footer>
    </div>
  </div>
</template>

<script setup>
import { ref, reactive } from 'vue';

const currentStep = ref(1);

const formData = reactive({
  personal: {
    fioRu: '',
    fioEn: '',
    cityRu: '',
    cityEn: '',
    email: '',
    phone: ''
  },
  education: {
    statusRu: '',
    statusEn: '',
    universityRu: '',
    universityEn: '',
    departmentRu: '',
    departmentEn: ''
  },
  thesis: {
    titleRu: '',
    titleEn: '',
    coauthorsRu: '',
    coauthorsEn: '',
    annotationRu: '',
    annotationEn: '',
    file: null
  }
});

const validateStep = (step) => {
  if (step === 1) {
    const { fioRu, fioEn, cityRu, cityEn, email } = formData.personal;
    return fioRu && fioEn && cityRu && cityEn && email;
  }
  if (step === 2) {
    const { statusRu, statusEn, universityRu, universityEn, departmentRu, departmentEn } = formData.education;
    return statusRu && statusEn && universityRu && universityEn && departmentRu && departmentEn;
  }
  if (step === 3) {
    const { titleRu, titleEn, annotationRu, annotationEn, file } = formData.thesis;
    return titleRu && titleEn && annotationRu && annotationEn && file;
  }
  return false;
};

const nextStep = () => {
  if (validateStep(currentStep.value)) {
    if (currentStep.value < 3) {
      currentStep.value++;
    }
  } else {
    alert('Пожалуйста, заполните все обязательные поля.');
  }
};

const prevStep = () => {
  if (currentStep.value > 1) {
    currentStep.value--;
  }
};

const submitForm = () => {
  if (validateStep(3)) {
    alert('Форма успешно отправлена!');
    console.log(JSON.parse(JSON.stringify(formData)));
    // Здесь будет логика отправки данных на сервер
  } else {
    alert('Пожалуйста, заполните все обязательные поля на последнем шаге.');
  }
};

const handleFileUpload = (event) => {
    const file = event.target.files[0];
    if (file) {
        formData.thesis.file = file;
    }
}
</script>

<style scoped lang="scss">
$main-color: #002166;
$light-main-color: #5cadff;
$dangerous: #e63946;
$black: #2d3748;
$gray: #7a7a7a;
$dark-white: #f7f9fc;
$white: #ffffff;

.form-wrapper {
  display: flex;
  justify-content: center;
  align-items: flex-start;
  min-height: 100vh;
  background-color: $dark-white; // Темный фон как в превью
  padding: 40px 20px;
}

.form-card {
  width: 100%;
  max-width: 440px;
  padding: 40px 32px;
  background-color: $white;
  border-radius: 40px; // radius 'xxl'
  box-shadow: 0 10px 40px rgba(0, 0, 0, 0.1);
}

.title {
  text-align: center;
  font-size: 32px;
  font-weight: 700;
  color: #333;
  margin: 0 0 32px 0;
  line-height: 1.1;
}

.form-group {
  margin-bottom: 24px;
  text-align: left;
}

.form-label {
  display: block;
  font-size: 16px; // Уменьшил до 16 для лучшей читаемости длинных форм
  font-weight: 500;
  color: #4a4a4a;
  margin-bottom: 8px;

  .required { color: $dangerous; }
}

.hint {
  font-size: 12px;
  color: $gray;
  margin: -4px 0 8px 0;
  line-height: 1.4;
}

input, textarea, select {
  width: 100%;
  padding: 16px 20px;
  background-color: $dark-white;
  border: 1px solid transparent;
  border-radius: 16px; // radius 'm'
  font-size: 16px;
  color: $black;
  box-sizing: border-box;
  transition: all 0.2s ease;

  &::placeholder { color: $gray; }
  &:focus {
    outline: none;
    background-color: $white;
    border-color: $light-main-color;
  }
}

textarea {
  resize: none;
  min-height: 100px;
}

.select-wrapper {
  position: relative;
  
  select {
    appearance: none;
    padding-right: 48px;
    cursor: pointer;
    &:invalid { color: $gray; }
  }

  .select-arrow {
    position: absolute;
    right: 16px;
    top: 50%;
    transform: translateY(-50%);
    pointer-events: none;
    color: $gray;
  }
}

.file-upload-area {
  position: relative;
  width: 100%;
  height: 120px;
  background-color: $dark-white;
  border-radius: 16px;
  display: flex;
  align-items: center;
  justify-content: center;
  cursor: pointer;
  border: 2px dashed transparent;
  transition: border 0.2s;

  &:hover { border-color: $light-main-color; }
}

.file-input {
  position: absolute;
  width: 100%;
  height: 100%;
  opacity: 0;
  cursor: pointer;
}

.plus-icon {
  font-size: 40px;
  color: $gray;
  font-weight: 300;
}

.file-name {
  font-size: 14px;
  color: $main-color;
  font-weight: 500;
}

.navigation-buttons {
  display: flex;
  flex-direction: column;
  gap: 12px;
  margin-top: 32px;
}

.btn-primary {
  width: 100%;
  padding: 18px;
  background-color: $main-color;
  color: $white;
  border: none;
  border-radius: 16px;
  font-size: 18px;
  font-weight: 600;
  cursor: pointer;
  transition: background-color 0.2s;

  &:hover { background-color: lighten($main-color, 10%); }
}

.btn-secondary {
  width: 100%;
  padding: 18px;
  background-color: $dark-white;
  color: $gray;
  border: none;
  border-radius: 16px;
  font-size: 18px;
  font-weight: 500;
  cursor: pointer;

  &:hover { background-color: darken($dark-white, 5%); }
}

.form-footer {
  margin-top: 32px;
  text-align: center;
  font-size: 13px;
  color: $gray;
  line-height: 1.5;
  
  a {
    color: $gray;
    text-decoration: none;
    &:hover { text-decoration: underline; }
  }
}
</style>