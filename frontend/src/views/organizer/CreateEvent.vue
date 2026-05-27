<template>
  <div class="page-container">
    <h1 class="page-title">Создание мероприятия</h1>

    <form @submit.prevent="handleSubmit" class="event-form">
      
      <div class="form-section base-info-grid">
        <div class="field col-span-6">
          <label>Название мероприятия: <span class="required">*</span></label>
          <input type="text" v-model="form.title" placeholder="Выберите секцию" class="ui-input" />
        </div>
        
        <div class="field col-span-2">
          <label>Тип мероприятия: <span class="required">*</span></label>
          <div class="select-wrapper blue">
            <select v-model="form.type" class="ui-select">
              <option value="conference">Конференция</option>
            </select>
          </div>
        </div>

        <div class="field col-span-2">
          <label>Формат участия: <span class="required">*</span></label>
          <div class="select-wrapper dark-blue">
            <select v-model="form.format" class="ui-select">
              <option value="online">Онлайн</option>
            </select>
          </div>
        </div>

        <div class="field col-span-2">
          <label>Индексация: <span class="required">*</span></label>
          <div class="select-wrapper black">
            <select v-model="form.indexing" class="ui-select">
              <option value="wos">Web of Science</option>
            </select>
          </div>
        </div>

        <div class="field col-span-3">
          <label>Город: <span class="required">*</span></label>
          <div class="select-wrapper">
            <select v-model="form.city" class="ui-select">
              <option value="" disabled selected>Выберите город</option>
            </select>
          </div>
        </div>

        <div class="field col-span-3">
          <label>Адрес: <span class="required">*</span></label>
          <input type="text" v-model="form.address" placeholder="Введите адрес" class="ui-input" />
        </div>

        <div class="field col-span-1-5">
          <label>Дата начала: <span class="required">*</span></label>
          <input type="text" v-model="form.startDate" placeholder="ДД.ММ.ГГГГ" class="ui-input" />
        </div>

        <div class="field col-span-1-5">
          <label>Время начала: <span class="required">*</span></label>
          <input type="text" v-model="form.startTime" placeholder="00:00" class="ui-input" />
        </div>

        <div class="field col-span-1-5">
          <label>Дата окончания: <span class="required">*</span></label>
          <input type="text" v-model="form.endDate" placeholder="ДД.ММ.ГГГГ" class="ui-input" />
        </div>

        <div class="field col-span-1-5">
          <label>Время окончания: <span class="required">*</span></label>
          <input type="text" v-model="form.endTime" placeholder="00:00" class="ui-input" />
        </div>

        <div class="field col-span-12">
          <label>Описание: <span class="required">*</span></label>
          <input type="text" v-model="form.description" placeholder="Описание вашего мероприятия" class="ui-input" />
        </div>
      </div>

      <h2 class="section-title">Требования</h2>
      <div class="form-section req-grid">
        
        <div class="field col-span-6">
          <label>Тип: <span class="required">*</span></label>
          <span class="hint">Например: требования к оформлению тезисов</span>
          <input type="text" v-model="form.reqType" placeholder="Наименование" class="ui-input" />
        </div>

        <div class="field col-span-6 row-span-2">
          <label>Описание: <span class="required">*</span></label>
          <input type="text" v-model="form.reqDescription" placeholder="Volkov Oleg Olegovich" class="ui-input mb-4" />
          
          <div class="file-upload">
            <span class="hint">Вы можете добавить файл. Допускается формат pdf, doc.<br>Размер загружаемого файла не более 5МБ</span>
            <button type="button" class="upload-btn">
              <svg width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                <circle cx="12" cy="12" r="10"></circle>
                <line x1="12" y1="8" x2="12" y2="16"></line>
                <line x1="8" y1="12" x2="16" y2="12"></line>
              </svg>
            </button>
          </div>
        </div>

        <div class="field col-span-3">
          <label>Статус (на русском): <span class="required">*</span></label>
          <div class="select-wrapper">
            <select v-model="form.statusRu" class="ui-select"><option>Выберите курс/должность/степень</option></select>
          </div>
        </div>
        <div class="field col-span-3">
          <label>Статус (на английском): <span class="required">*</span></label>
          <div class="select-wrapper">
            <select v-model="form.statusEn" class="ui-select"><option>Выберите курс/должность/степень</option></select>
          </div>
        </div>

        <div class="field col-span-3">
          <label>ВУЗ (на русском): <span class="required">*</span></label>
          <input type="text" v-model="form.universityRu" placeholder="Введите название вашего ВУЗа" class="ui-input" />
        </div>
        <div class="field col-span-3">
          <label>ВУЗ (на английском): <span class="required">*</span></label>
          <input type="text" v-model="form.universityEn" placeholder="Введите название вашего ВУЗа" class="ui-input" />
        </div>

        <div class="field col-span-3">
          <label>Город (на английском): <span class="required">*</span></label>
          <div class="select-wrapper">
            <select v-model="form.cityEn" class="ui-select"><option>Выберите курс/должность/степень</option></select>
          </div>
        </div>
        <div class="field col-span-3">
          <label>Город (на русском): <span class="required">*</span></label>
          <div class="select-wrapper">
            <select v-model="form.cityRuReq" class="ui-select"><option>Выберите город</option></select>
          </div>
        </div>

        <div class="field col-span-3">
          <label>Подразделение (на русском): <span class="required">*</span></label>
          <span class="hint">Структурное подразделение вашего ВУЗа</span>
          <input type="text" v-model="form.departmentRu" placeholder="Введите название подразделения" class="ui-input" />
        </div>
        <div class="field col-span-3">
          <label>Подразделение (на английском): <span class="required">*</span></label>
          <span class="hint">Структурное подразделение вашего ВУЗа</span>
          <input type="text" v-model="form.departmentEn" placeholder="Введите название подразделения" class="ui-input" />
        </div>

      </div>

      <h2 class="section-title">Информация о работе</h2>
      <div class="form-section work-grid">
        
        <div class="field col-span-4">
          <label>Введите УДК: <span class="required">*</span></label>
          <span class="hint">Справочник по УДК</span>
          <input type="text" v-model="form.udk" placeholder="00.00" class="ui-input" />
        </div>

        <div class="field col-span-4">
          <label>Названия тезисов (на русском): <span class="required">*</span></label>
          <input type="text" v-model="form.thesisTitleRu" placeholder="Введите название вашего ВУЗа" class="ui-input mt-auto" />
        </div>

        <div class="field col-span-4">
          <label>Названия тезисов (на английском): <span class="required">*</span></label>
          <input type="text" v-model="form.thesisTitleEn" placeholder="Введите название вашего ВУЗа" class="ui-input mt-auto" />
        </div>

        <div class="field col-span-3">
          <label>Соавторы (на русском):</label>
          <span class="hint">Для каждого соавтора указать: ФИО, курс/должность/степень, структурное подразделение вуза, вуз, город и email</span>
          <textarea v-model="form.coauthorsRu" placeholder="Введите информацию о соавторах" class="ui-textarea"></textarea>
        </div>

        <div class="field col-span-3">
          <label>Соавторы (на английском):</label>
          <span class="hint">Для каждого соавтора указать: ФИО, курс/должность/степень, структурное подразделение вуза, вуз, город и email</span>
          <textarea v-model="form.coauthorsEn" placeholder="Введите информацию о соавторах" class="ui-textarea"></textarea>
        </div>

        <div class="field col-span-3">
          <label>Аннотация (на русском): <span class="required">*</span></label>
          <textarea v-model="form.abstractRu" placeholder="Введите название вашего ВУЗа" class="ui-textarea mt-auto"></textarea>
        </div>

        <div class="field col-span-3">
          <label>Аннотация (на английском): <span class="required">*</span></label>
          <textarea v-model="form.abstractEn" placeholder="Введите название вашего ВУЗа" class="ui-textarea mt-auto"></textarea>
        </div>

      </div>

      <div class="actions">
        <button type="button" class="btn btn-back">Назад</button>
        <button type="submit" class="btn btn-submit">Отправить</button>
      </div>

    </form>
    
    <div class="support-text">
      По всем вопросам можете обращаться: adminexample@gmail.com
    </div>
  </div>
</template>

<script setup>
import { reactive } from 'vue';

const form = reactive({
  title: '', type: 'conference', format: 'online', indexing: 'wos',
  city: '', address: '', startDate: '', startTime: '', endDate: '', endTime: '', description: '',
  reqType: '', reqDescription: '', statusRu: '', statusEn: '', universityRu: '', universityEn: '',
  cityEn: '', cityRuReq: '', departmentRu: '', departmentEn: '',
  udk: '', thesisTitleRu: '', thesisTitleEn: '', coauthorsRu: '', coauthorsEn: '', abstractRu: '', abstractEn: ''
});

const handleSubmit = () => {
  console.log('Form Submitted', form);
};
</script>

<style scoped>
/* Цветовые переменные на основе макета */
:root {
  --c-main-blue: #003399;
  --c-light-blue: #5b7fff;
  --c-dark-blue: #002b80;
  --c-black: #1a1a1a;
  --c-red: #e60000;
  --c-bg-input: #f7f7f7;
  --c-text-main: #333;
  --c-text-hint: #888;
}

.page-container {
  max-width: 1400px;
  margin: 0 auto;
  padding: 40px 20px;
  font-family: sans-serif;
}

.page-title {
  color: var(--c-main-blue);
  font-size: 32px;
  margin-bottom: 40px;
}

.section-title {
  color: var(--c-main-blue);
  font-size: 24px;
  margin-top: 40px;
  margin-bottom: 24px;
}

/* Сетки */
.form-section {
  display: grid;
  gap: 24px 20px;
  margin-bottom: 32px;
}

.base-info-grid { grid-template-columns: repeat(12, 1fr); }
.req-grid { grid-template-columns: repeat(12, 1fr); }
.work-grid { grid-template-columns: repeat(12, 1fr); }

/* Утилиты колонок */
.col-span-12 { grid-column: span 12; }
.col-span-6 { grid-column: span 6; }
.col-span-4 { grid-column: span 4; }
.col-span-3 { grid-column: span 3; }
.col-span-2 { grid-column: span 2; }
.col-span-1-5 { grid-column: span 3; } /* В сетке из 12, 1.5 это примерно 3 колонки (если делить по-другому), здесь адаптировано для дат */

@media (max-width: 1024px) {
  .col-span-1-5 { grid-column: span 6; }
  .col-span-2, .col-span-3, .col-span-4 { grid-column: span 6; }
}
@media (max-width: 600px) {
  .field { grid-column: span 12 !important; }
}

/* Поля ввода */
.field {
  display: flex;
  flex-direction: column;
}

label {
  font-size: 14px;
  color: var(--c-text-main);
  margin-bottom: 4px;
}

.required { color: var(--c-red); }

.hint {
  font-size: 11px;
  color: var(--c-text-hint);
  margin-bottom: 8px;
  line-height: 1.3;
}

.ui-input, .ui-select, .ui-textarea {
  width: 100%;
  background-color: var(--c-bg-input);
  border: none;
  border-radius: 8px;
  padding: 14px 16px;
  font-size: 14px;
  color: var(--c-text-main);
  outline: none;
  box-sizing: border-box;
}

.ui-textarea {
  min-height: 100px;
  resize: vertical;
}

.mt-auto { margin-top: auto; }
.mb-4 { margin-bottom: 16px; }

/* Кастомные Select'ы для шапки (цветные) */
.select-wrapper {
  position: relative;
}
.select-wrapper::after {
  content: '⌄';
  position: absolute;
  right: 16px;
  top: 50%;
  transform: translateY(-50%);
  pointer-events: none;
  color: var(--c-text-hint);
}

.select-wrapper.blue .ui-select { background-color: var(--c-light-blue); color: white; }
.select-wrapper.dark-blue .ui-select { background-color: var(--c-dark-blue); color: white; }
.select-wrapper.black .ui-select { background-color: var(--c-black); color: white; }
.select-wrapper.blue::after, .select-wrapper.dark-blue::after, .select-wrapper.black::after { color: white; }

/* Загрузка файла */
.file-upload {
  display: flex;
  flex-direction: column;
}
.upload-btn {
  background-color: var(--c-bg-input);
  border: none;
  border-radius: 8px;
  padding: 20px;
  cursor: pointer;
  color: var(--c-text-hint);
  display: flex;
  justify-content: center;
  align-items: center;
  transition: background 0.2s;
}
.upload-btn:hover { background-color: #e2e2e2; }

/* Кнопки и Футер */
.actions {
  display: grid;
  grid-template-columns: 1fr 1fr;
  gap: 20px;
  margin-top: 40px;
}

.btn {
  padding: 16px;
  border-radius: 8px;
  font-size: 16px;
  font-weight: 500;
  cursor: pointer;
  border: none;
  transition: opacity 0.2s;
}

.btn:hover { opacity: 0.9; }

.btn-back {
  background-color: var(--c-bg-input);
  color: var(--c-text-main);
}

.btn-submit {
  background-color: var(--c-dark-blue);
  color: white;
}

.support-text {
  text-align: center;
  font-size: 12px;
  color: var(--c-text-hint);
  margin-top: 24px;
}
</style>