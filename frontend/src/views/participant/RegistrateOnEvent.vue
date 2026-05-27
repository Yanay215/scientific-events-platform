<template>
  <div class="page-container">
    
    <header class="header">
      <div class="logo">ЛОГО</div>
      <button type="button" class="icon-btn">
        <svg width="18" height="18" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
          <circle cx="12" cy="8" r="4"/>
          <path d="M4 20c0-4 3.6-7 8-7s8 3 8 7"/>
        </svg>
      </button>
    </header>

    <div v-if="submitted" class="success-screen">
      <div class="success-icon">
        <svg width="32" height="32" viewBox="0 0 24 24" fill="none" stroke="#16a34a" stroke-width="2.5">
          <polyline points="20 6 9 17 4 12"/>
        </svg>
      </div>
      <div class="success-title">Заявка отправлена!</div>
      <div class="success-sub">
        Ваша регистрация на конференцию принята. Ожидайте подтверждения на указанный email: <strong>{{ form.email }}</strong>.
      </div>
      <button class="btn-primary" style="max-width:220px;height:46px" @click="submitted = false">На главную</button>
    </div>

    <form v-else @submit.prevent="handleSubmit" class="event-form">
      
      <div class="desktop-only">
        <div class="desktop-wrap">
          <h1 class="page-title-desktop">Регистрация на мероприятие</h1>

          <div class="form-block mb24">
            <div class="f-field">
              <div class="f-label">Выберите секцию конференции: <span class="req">*</span></div>
              <select class="f-select" :class="{ err: de.section }" v-model="form.section">
                <option value="">Выберите секцию</option>
                <option v-for="s in sections" :key="s">{{ s }}</option>
              </select>
              <span class="err-msg" v-if="de.section">Обязательное поле</span>
            </div>
          </div>

          <div class="form-block">
            <div class="sec-title">Проверьте ваши личные данные</div>
            
            <div class="g4 mb16">
              <div class="f-field">
                <div class="f-label">ФИО (на русском): <span class="req">*</span></div>
                <input class="f-input" :class="{ err: de.fioRu }" v-model="form.fioRu" placeholder="Волков Олег Олегович">
                <span class="err-msg" v-if="de.fioRu">Обязательное поле</span>
              </div>
              <div class="f-field">
                <div class="f-label">ФИО (на английском): <span class="req">*</span></div>
                <input class="f-input" :class="{ err: de.fioEn }" v-model="form.fioEn" placeholder="Volkov Oleg Olegovich">
                <span class="err-msg" v-if="de.fioEn">Обязательное поле</span>
              </div>
              <div class="f-field">
                <div class="f-label">Почта: <span class="req">*</span></div>
                <input class="f-input" :class="{ err: de.email }" v-model="form.email" type="email" placeholder="example@mail.com">
                <span class="err-msg" v-if="de.email">Некорректный email</span>
              </div>
              <div class="f-field">
                <div class="f-label">Ваш номер телефона:</div>
                <input class="f-input" v-model="form.phone" placeholder="+7 (999) 999-99-99">
              </div>
            </div>

            <div class="g4 mb16">
              <div class="f-field">
                <div class="f-label">Статус (на русском): <span class="req">*</span></div>
                <select class="f-select" :class="{ err: de.statusRu }" v-model="form.statusRu">
                  <option value="">Выберите курс/должность/степень</option>
                  <option v-for="s in statusesRu" :key="s">{{ s }}</option>
                </select>
                <span class="err-msg" v-if="de.statusRu">Обязательное поле</span>
              </div>
              <div class="f-field">
                <div class="f-label">Статус (на английском): <span class="req">*</span></div>
                <select class="f-select" v-model="form.statusEn">
                  <option value="">Выберите курс/должность/степень</option>
                  <option v-for="s in statusesEn" :key="s">{{ s }}</option>
                </select>
              </div>
              <div class="f-field">
                <div class="f-label">ВУЗ (на русском): <span class="req">*</span></div>
                <input class="f-input" :class="{ err: de.vuzRu }" v-model="form.vuzRu" placeholder="Введите название вашего ВУЗа">
                <span class="err-msg" v-if="de.vuzRu">Обязательное поле</span>
              </div>
              <div class="f-field">
                <div class="f-label">ВУЗ (на английском): <span class="req">*</span></div>
                <input class="f-input" v-model="form.vuzEn" placeholder="University name in English">
              </div>
            </div>

            <div class="g4 mb16">
              <div class="f-field">
                <div class="f-label">Город (на русском): <span class="req">*</span></div>
                <select class="f-select" :class="{ err: de.cityRu }" v-model="form.cityRu">
                  <option value="">Выберите город</option>
                  <option v-for="c in cities" :key="c">{{ c }}</option>
                </select>
                <span class="err-msg" v-if="de.cityRu">Обязательное поле</span>
              </div>
              <div class="f-field">
                <div class="f-label">Город (на английском): <span class="req">*</span></div>
                <select class="f-select" v-model="form.cityEn">
                  <option value="">Выберите город</option>
                  <option v-for="c in citiesEn" :key="c">{{ c }}</option>
                </select>
              </div>
              <div class="f-field">
                <div class="f-label">Подразделение (на русском): <span class="req">*</span></div>
                <div class="f-hint">Структурное подразделение вашего ВУЗа</div>
                <input class="f-input" :class="{ err: de.subdivRu }" v-model="form.subdivRu" placeholder="Введите название подразделения">
                <span class="err-msg" v-if="de.subdivRu">Обязательное поле</span>
              </div>
              <div class="f-field">
                <div class="f-label">Подразделение (на английском): <span class="req">*</span></div>
                <div class="f-hint">Structural subdivision</div>
                <input class="f-input" v-model="form.subdivEn" placeholder="Department name in English">
              </div>
            </div>

            <template v-if="isStudent">
              <div class="g3 mb16">
                <div class="f-field">
                  <div class="f-label">Ступень обучения (на русском): <span class="req">*</span></div>
                  <select class="f-select" :class="{ err: de.degreeRu }" v-model="form.degreeRu">
                    <option value="">Выберите ступень обучения</option>
                    <option>Бакалавриат</option>
                    <option>Специалитет</option>
                    <option>Магистратура</option>
                    <option>Aспирантура</option>
                  </select>
                  <span class="err-msg" v-if="de.degreeRu">Обязательное поле</span>
                </div>
                <div class="f-field">
                  <div class="f-label">Ступень обучения (на английском): <span class="req">*</span></div>
                  <select class="f-select" v-model="form.degreeEn">
                    <option value="">Выберите ступень обучения</option>
                    <option>Bachelor</option>
                    <option>Specialist</option>
                    <option>Master</option>
                    <option>PhD</option>
                  </select>
                </div>
                <div class="f-field">
                  <div class="f-label">Курс: <span class="req">*</span></div>
                  <select class="f-select" :class="{ err: de.course }" v-model="form.course">
                    <option value="">Выберите курс</option>
                    <option v-for="n in 6" :key="n" :value="n">{{ n }} курс</option>
                  </select>
                  <span class="err-msg" v-if="de.course">Обязательное поле</span>
                </div>
              </div>
            </template>

            <template v-if="isTeacher">
              <div class="g4 mb16">
                <div class="f-field">
                  <div class="f-label">Степень (на русском):</div>
                  <input class="f-input" v-model="form.degree" placeholder="К.т.н., доцент">
                </div>
                <div class="f-field">
                  <div class="f-label">Степень (на английском):</div>
                  <input class="f-input" v-model="form.degreeEnT" placeholder="PhD">
                </div>
                <div class="f-field">
                  <div class="f-label">Научное звание (на русском):</div>
                  <input class="f-input" v-model="form.title" placeholder="Профессор">
                </div>
                <div class="f-field">
                  <div class="f-label">Научное звание (на английском):</div>
                  <input class="f-input" v-model="form.titleEn" placeholder="Professor">
                </div>
              </div>
              <div class="mb16">
                <label class="f-check">
                  <input type="checkbox" v-model="form.youngScientist">
                  <span>Молодой учёный</span>
                </label>
              </div>
            </template>
          </div>

          <div class="form-block">
            <div class="sec-title">Информация о работе</div>
            <div class="g3 mb16">
              <div class="f-field">
                <div class="f-label">Введите УДК: <span class="req">*</span></div>
                <div class="f-hint">Справочник по УДК</div>
                <input class="f-input" :class="{ err: de.udk }" v-model="form.udk" placeholder="00.00">
                <span class="err-msg" v-if="de.udk">Обязательное поле</span>
              </div>
              <div class="f-field">
                <div class="f-label">Название тезисов (на русском): <span class="req">*</span></div>
                <input class="f-input" :class="{ err: de.titleRu }" v-model="form.thesisTitleRu" placeholder="Введите название тезисов">
                <span class="err-msg" v-if="de.titleRu">Обязательное поле</span>
              </div>
              <div class="f-field">
                <div class="f-label">Название тезисов (на английском): <span class="req">*</span></div>
                <input class="f-input" :class="{ err: de.titleEn }" v-model="form.thesisTitleEn" placeholder="Thesis title in English">
                <span class="err-msg" v-if="de.titleEn">Обязательное поле</span>
              </div>
            </div>

            <template v-if="isStudent">
              <div class="g2 mb16">
                <div class="f-field">
                  <div class="f-label">Научный руководитель (на русском):</div>
                  <div class="f-hint">Указать: ФИО, должность, ВУЗ, город и email</div>
                  <textarea class="f-textarea" v-model="form.supervisorRu" placeholder="Введите информацию..."></textarea>
                </div>
                <div class="f-field">
                  <div class="f-label">Научный руководитель (на английском):</div>
                  <div class="f-hint">Specify: Full Name, position, University, city and email</div>
                  <textarea class="f-textarea" v-model="form.supervisorEn" placeholder="Enter info..."></textarea>
                </div>
              </div>
            </template>
            <template v-else>
              <div class="g2 mb16">
                <div class="f-field">
                  <div class="f-label">Соавторы (на русском):</div>
                  <div class="f-hint">Для каждого соавтора указать ФИО, должность, ВУЗ и email</div>
                  <textarea class="f-textarea" v-model="form.coAuthorsRu" placeholder="Введите информацию о соавторах"></textarea>
                </div>
                <div class="f-field">
                  <div class="f-label">Соавторы (на английском):</div>
                  <div class="f-hint">For each co-author specify Full Name, position, University and email</div>
                  <textarea class="f-textarea" v-model="form.coAuthorsEn" placeholder="Enter co-authors info..."></textarea>
                </div>
              </div>
            </template>

            <div class="g2 mb16">
              <div class="f-field">
                <div class="f-label">Аннотация (на русском): <span class="req">*</span></div>
                <textarea class="f-textarea" :class="{ err: de.annotRu }" v-model="form.annotationRu" placeholder="Краткая аннотация вашей работы..."></textarea>
                <span class="err-msg" v-if="de.annotRu">Обязательное поле</span>
              </div>
              <div class="f-field">
                <div class="f-label">Аннотация (на английском): <span class="req">*</span></div>
                <textarea class="f-textarea" :class="{ err: de.annotEn }" v-model="form.annotationEn" placeholder="Abstract in English..."></textarea>
                <span class="err-msg" v-if="de.annotEn">Обязательное поле</span>
              </div>
            </div>

            <div class="f-field">
              <div class="f-label">Текст тезисов: <span class="req">*</span></div>
              <div class="f-hint">Допускается формат doc/docx. Размер загружаемого файла не более 5МБ</div>
              <span class="err-msg" v-if="fileErr" style="margin-bottom:6px">{{ fileErr }}</span>
              
              <div class="upload-zone" :class="{ 'has-file': form.thesisFile && !fileErr, 'upload-err': de.file || !!fileErr }" @click="$refs.fileInputDesktop.click()">
                <input type="file" ref="fileInputDesktop" accept=".doc,.docx" @change="handleFile" style="display:none">
                <svg class="upload-icon" width="28" height="28" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                  <path d="M21 15v4a2 2 0 0 1-2 2H5a2 2 0 0 1-2-2v-4M17 8l-5-5-5 5M12 3v12"/>
                </svg>
                <span v-if="form.thesisFile" class="upload-filename">Файл: {{ form.thesisFile }}</span>
                <span v-else class="upload-hint">Кликните для выбора файла или перетащите его сюда</span>
              </div>
              <span class="err-msg" v-if="de.file && !form.thesisFile">Необходимо загрузить файл тезисов</span>
            </div>
          </div>

          <div class="desktop-footer">
            <button type="button" class="btn-secondary" @click="resetForm">Очистить форму</button>
            <button type="submit" class="btn-primary">Отправить заявку</button>
          </div>
          <div class="contact-line">Возникли вопросы? Обратитесь в техническую поддержку конференции</div>
        </div>
      </div>

      <div class="mobile-only">
        <div class="mobile-wrap">
          <h1 class="page-title-mobile">Регистрация на мероприятие</h1>

          <transition name="slide" mode="out-in">
            <div :key="mobStep" class="mob-step">
              
              <div v-if="mobStep === 0">
                <div class="sec-title">Шаг 1: Направление</div>
                <div class="f-field mb16">
                  <div class="f-label">Выберите секцию конференции: <span class="req">*</span></div>
                  <select class="f-select" :class="{ err: me.section }" v-model="form.section">
                    <option value="">Выберите секцию</option>
                    <option v-for="s in sections" :key="s">{{ s }}</option>
                  </select>
                  <span class="err-msg" v-if="me.section">Обязательное поле</span>
                </div>
                <div class="f-field mb16">
                  <div class="f-label">Почта: <span class="req">*</span></div>
                  <input class="f-input" :class="{ err: me.email }" v-model="form.email" type="email" placeholder="example@mail.com">
                  <span class="err-msg" v-if="me.email">Некорректный email</span>
                </div>
                
                <div class="mob-footer">
                  <button type="button" class="btn-primary" @click="mobNext(0)">Далее</button>
                </div>
              </div>

              <div v-if="mobStep === 1">
                <div class="sec-title">Шаг 2: Личные данные</div>
                <div class="f-field mb16">
                  <div class="f-label">ФИО (на русском): <span class="req">*</span></div>
                  <input class="f-input" :class="{ err: me.fioRu }" v-model="form.fioRu">
                </div>
                <div class="f-field mb16">
                  <div class="f-label">Статус (на русском): <span class="req">*</span></div>
                  <select class="f-select" :class="{ err: me.statusRu }" v-model="form.statusRu">
                    <option value="">Выберите статус</option>
                    <option v-for="s in statusesRu" :key="s">{{ s }}</option>
                  </select>
                </div>
                <div class="f-field mb16">
                  <div class="f-label">ВУЗ (на русском): <span class="req">*</span></div>
                  <input class="f-input" :class="{ err: me.vuzRu }" v-model="form.vuzRu">
                </div>
                <div class="f-field mb16">
                  <div class="f-label">Подразделение (на русском): <span class="req">*</span></div>
                  <input class="f-input" :class="{ err: me.subdivRu }" v-model="form.subdivRu">
                </div>

                <template v-if="isStudent">
                  <div class="f-field mb16">
                    <div class="f-label">Ступень обучения (RU): <span class="req">*</span></div>
                    <select class="f-select" :class="{ err: me.degreeRu }" v-model="form.degreeRu">
                      <option value="">Выберите ступень</option>
                      <option>Бакалавриат</option>
                      <option>Специалитет</option>
                      <option>Магистратура</option>
                      <option>Аспирантура</option>
                    </select>
                  </div>
                  <div class="f-field mb16">
                    <div class="f-label">Курс: <span class="req">*</span></div>
                    <select class="f-select" :class="{ err: me.course }" v-model="form.course">
                      <option value="">Выберите курс</option>
                      <option v-for="n in 6" :key="n" :value="n">{{ n }}</option>
                    </select>
                  </div>
                </template>

                <div class="mob-footer">
                  <button type="button" class="btn-primary" @click="mobNext(1)">Далее</button>
                  <button type="button" class="btn-secondary" @click="mobStep = 0">Назад</button>
                </div>
              </div>

              <div v-if="mobStep === 2">
                <div class="sec-title">Шаг 3: Тезисы и аннотация</div>
                <div class="f-field mb16">
                  <div class="f-label">Название тезисов (RU): <span class="req">*</span></div>
                  <input class="f-input" :class="{ err: me.titleRu }" v-model="form.thesisTitleRu">
                </div>
                <div class="f-field mb16">
                  <div class="f-label">Название тезисов (EN): <span class="req">*</span></div>
                  <input class="f-input" :class="{ err: me.titleEn }" v-model="form.thesisTitleEn">
                </div>
                <div class="f-field mb16">
                  <div class="f-label">Аннотация (RU): <span class="req">*</span></div>
                  <textarea class="f-textarea" :class="{ err: me.annotRu }" v-model="form.annotationRu"></textarea>
                </div>
                <div class="f-field mb16">
                  <div class="f-label">Аннотация (EN): <span class="req">*</span></div>
                  <textarea class="f-textarea" :class="{ err: me.annotEn }" v-model="form.annotationEn"></textarea>
                </div>

                <div class="f-field mb16">
                  <div class="f-label">Файл тезисов: <span class="req">*</span></div>
                  <div class="upload-zone" :class="{ 'has-file': form.thesisFile, 'upload-err': me.file }" @click="$refs.fileInputMobile.click()">
                    <input type="file" ref="fileInputMobile" accept=".doc,.docx" @change="handleFile" style="display:none">
                    <span v-if="form.thesisFile" class="upload-filename">{{ form.thesisFile }}</span>
                    <span v-else class="upload-hint">Выбрать файл (.doc/.docx)</span>
                  </div>
                </div>

                <div class="mob-footer">
                  <button type="button" class="btn-primary" @click="submitMobile">Отправить заявку</button>
                  <button type="button" class="btn-secondary" @click="mobStep = 1">Назад</button>
                </div>
              </div>

            </div>
          </transition>
        </div>
      </div>

    </form>
  </div>
</template>

<script setup>
import { ref, reactive, computed } from 'vue';

// Справочники полей конференции из registration.html
const sections = [
  'Информационные технологии и интеллектуальные системы',
  'Дизайн, архитектура и урбанистика',
  'Цифровая экономика и управление инновациями'
];

const statusesRu = ['Студент', 'Аспирант', 'Преподаватель / Ученый', 'Представитель индустрии'];
const statusesEn = ['Student', 'Postgraduate', 'Teacher / Scientist', 'Industry Representative'];
const cities = ['Москва', 'Санкт-Петербург', 'Екатеринбург', 'Новосибирск', 'Казань'];
const citiesEn = ['Moscow', 'Saint-Petersburg', 'Yekaterinburg', 'Novosibirsk', 'Kazan'];

// Общие состояния управления формой
const submitted = ref(false);
const mobStep = ref(0);
const fileErr = ref('');

// Реактивный стейт формы в соответствии с HTML-шаблоном
const form = reactive({
  section: '',
  fioRu: '',
  fioEn: '',
  email: '',
  phone: '',
  statusRu: '',
  statusEn: '',
  vuzRu: '',
  vuzEn: '',
  cityRu: '',
  cityEn: '',
  subdivRu: '',
  subdivEn: '',
  degreeRu: '',
  degreeEn: '',
  course: '',
  degree: '',     // Степень для преподавателя
  degreeEnT: '',  // Степень для преподавателя (EN)
  title: '',     // Звание для преподавателя
  titleEn: '',   // Звание для преподавателя (EN)
  youngScientist: false,
  udk: '',
  thesisTitleRu: '',
  thesisTitleEn: '',
  supervisorRu: '',
  supervisorEn: '',
  coAuthorsRu: '',
  coAuthorsEn: '',
  annotationRu: '',
  annotationEn: '',
  thesisFile: null
});

// Стейты валидации (Desktop Errors / Mobile Errors)
const de = reactive({
  section: false, fioRu: false, fioEn: false, email: false, statusRu: false,
  vuzRu: false, cityRu: false, subdivRu: false, degreeRu: false, course: false,
  udk: false, titleRu: false, titleEn: false, annotRu: false, annotEn: false, file: false
});

const me = reactive({
  section: false, email: false, fioRu: false, statusRu: false, vuzRu: false,
  subdivRu: false, degreeRu: false, course: false, titleRu: false, titleEn: false,
  annotRu: false, annotEn: false, file: false
});

// Вычисляемые свойства для условного рендеринга типов участников
const isStudent = computed(() => form.statusRu && form.statusRu.toLowerCase().includes('студент'));
const isTeacher = computed(() => form.statusRu && (form.statusRu.toLowerCase().includes('преподаватель') || form.statusRu.toLowerCase().includes('аспирант')));

const clearDE = () => Object.keys(de).forEach(k => de[k] = false);
const clearME = () => Object.keys(me).forEach(k => me[k] = false);

// Логика обработки и валидации загружаемого файла
const handleFile = (e) => {
  fileErr.value = '';
  const file = e.target.files[0];
  if (!file) return;

  const ext = file.name.split('.').pop().toLowerCase();
  if (ext !== 'doc' && ext !== 'docx') {
    fileErr.value = 'Неверный формат файла. Разрешены только .doc и .docx';
    form.thesisFile = null;
    return;
  }
  if (file.size > 5 * 1024 * 1024) {
    fileErr.value = 'Размер файла превышает 5 МБ';
    form.thesisFile = null;
    return;
  }
  form.thesisFile = file.name;
};

// Валидация и отправка формы для десктопа
const handleSubmit = () => {
  clearDE();
  let ok = true;

  if (!form.section) { de.section = true; ok = false; }
  if (!form.fioRu) { de.fioRu = true; ok = false; }
  if (!form.fioEn) { de.fioEn = true; ok = false; }
  if (!form.email || !/\S+@\S+\.\S+/.test(form.email)) { de.email = true; ok = false; }
  if (!form.statusRu) { de.statusRu = true; ok = false; }
  if (!form.vuzRu) { de.vuzRu = true; ok = false; }
  if (!form.cityRu) { de.cityRu = true; ok = false; }
  if (!form.subdivRu) { de.subdivRu = true; ok = false; }
  if (isStudent.value && !form.degreeRu) { de.degreeRu = true; ok = false; }
  if (isStudent.value && !form.course) { de.course = true; ok = false; }
  if (!form.udk) { de.udk = true; ok = false; }
  if (!form.thesisTitleRu) { de.titleRu = true; ok = false; }
  if (!form.thesisTitleEn) { de.titleEn = true; ok = false; }
  if (!form.annotationRu) { de.annotRu = true; ok = false; }
  if (!form.annotationEn) { de.annotEn = true; ok = false; }
  if (!form.thesisFile) { de.file = true; ok = false; }

  if (ok) submitted.value = true;
};

// Логика перехода между шагами на мобильных устройствах
const mobNext = (step) => {
  clearME();
  let ok = true;
  if (step === 0) {
    if (!form.section) { me.section = true; ok = false; }
    if (!form.email || !/\S+@\S+\.\S+/.test(form.email)) { me.email = true; ok = false; }
  }
  if (step === 1) {
    if (!form.fioRu) { me.fioRu = true; ok = false; }
    if (!form.statusRu) { me.statusRu = true; ok = false; }
    if (!form.vuzRu) { me.vuzRu = true; ok = false; }
    if (!form.subdivRu) { me.subdivRu = true; ok = false; }
    if (isStudent.value && !form.degreeRu) { me.degreeRu = true; ok = false; }
    if (isStudent.value && !form.course) { me.course = true; ok = false; }
  }
  if (ok) mobStep.value = step + 1;
};

// Валидация мобильной отправки
const submitMobile = () => {
  clearME();
  let ok = true;
  if (!form.thesisTitleRu) { me.titleRu = true; ok = false; }
  if (!form.thesisTitleEn) { me.titleEn = true; ok = false; }
  if (!form.annotationRu) { me.annotRu = true; ok = false; }
  if (!form.annotationEn) { me.annotEn = true; ok = false; }
  if (!form.thesisFile) { me.file = true; ok = false; }

  if (ok) submitted.value = true;
};

const resetForm = () => {
  Object.keys(form).forEach(key => {
    if (typeof form[key] === 'boolean') form[key] = false;
    else form[key] = '';
  });
  form.thesisFile = null;
  fileErr.value = '';
  clearDE();
};
</script>

<style scoped>
/* Перенесенная палитра и переменные темы */
:root {
  --navy:         #1a2b5e;
  --navy-dark:    #0f1c42;
  --accent:       #1d4ed8;
  --bg:           #f5f6fa;
  --surface:      #ffffff;
  --border:       #e2e6f0;
  --text:         #1a1e2e;
  --muted:        #6b7490;
  --error:        #d0222a;
  --radius-sm:    8px;
  --radius-md:    12px;
  --radius-lg:    16px;
}

.page-container {
  background: #f5f6fa;
  min-height: 100vh;
  color: #1a1e2e;
}

/* ── HEADER ── */
.header {
  background: #ffffff; border-bottom: 1px solid #e2e6f0;
  padding: 0 40px; height: 60px;
  display: flex; align-items: center; justify-content: space-between;
  position: sticky; top: 0; z-index: 100;
}
.logo { font-size: 20px; font-weight: 700; color: #1a2b5e; }
.icon-btn {
  width: 38px; height: 38px; border-radius: 50%;
  border: 1px solid #e2e6f0; background: transparent;
  cursor: pointer; display: flex; align-items: center; justify-content: center;
  color: #6b7490; transition: background 0.15s;
}
.icon-btn:hover { background: #f5f6fa; }

/* ── SHARED FIELD STYLES ── */
.f-label {
  font-size: 14px; font-weight: 500; color: #1a1e2e;
  margin-bottom: 4px; display: flex; align-items: baseline; gap: 3px;
}
.f-hint { font-size: 12px; color: #6b7490; margin-bottom: 6px; line-height: 1.45; }
.req { color: #d0222a; }
.f-input, .f-select, .f-textarea {
  width: 100%; height: 44px; padding: 0 14px;
  border: 1px solid #e2e6f0; border-radius: 8px;
  background: #f5f6fa;
  font-size: 14px; color: #1a1e2e; outline: none; transition: border-color 0.15s;
}
.f-input:focus, .f-select:focus, .f-textarea:focus { border-color: #1d4ed8; background: #fff; }
.f-input.err, .f-select.err, .f-textarea.err { border-color: #d0222a; }
.f-textarea { height: 90px; padding: 10px 14px; resize: vertical; line-height: 1.5; }
.err-msg { font-size: 12px; color: #d0222a; margin-top: 4px; }
.f-field { display: flex; flex-direction: column; }

/* Checkbox */
.f-check { display: flex; align-items: center; gap: 8px; cursor: pointer; user-select: none; }
.f-check input[type=checkbox] { width: 17px; height: 17px; accent-color: #1d4ed8; cursor: pointer; }
.f-check span { font-size: 14px; color: #1a1e2e; }

/* File upload */
.upload-zone {
  border: 1.5px dashed #e2e6f0; border-radius: 8px;
  background: #f5f6fa; min-height: 100px;
  display: flex; flex-direction: column; align-items: center; justify-content: center;
  gap: 8px; cursor: pointer; transition: border-color 0.15s, background 0.15s;
  padding: 16px; position: relative;
}
.upload-zone:hover { border-color: #1d4ed8; background: #f0f4ff; }
.upload-zone.has-file { background: #f0f8f4; border-color: #4ade80; }
.upload-zone.upload-err { border-color: #d0222a; background: #fff5f5; }
.upload-icon { color: #6b7490; }
.upload-hint { font-size: 12px; color: #6b7490; text-align: center; }
.upload-filename { font-size: 13px; font-weight: 500; color: #16a34a; }

/* Section heading */
.sec-title {
  font-size: 18px; font-weight: 700; color: #1d4ed8;
  margin-bottom: 18px; margin-top: 8px;
}

/* ── DESKTOP GRID & LAYOUT ── */
.desktop-wrap { max-width: 1280px; margin: 0 auto; padding: 32px 40px 60px; }
.page-title-desktop { font-size: 26px; font-weight: 700; color: #1a2b5e; margin-bottom: 28px; }
.form-block {
  background: #ffffff; border: 1px solid #e2e6f0;
  border-radius: 16px; padding: 28px 32px; margin-bottom: 20px;
}
.g2 { display: grid; grid-template-columns: 1fr 1fr; gap: 16px 24px; }
.g3 { display: grid; grid-template-columns: 1fr 1fr 1fr; gap: 16px 24px; }
.g4 { display: grid; grid-template-columns: 1fr 1fr 1fr 1fr; gap: 16px 24px; }
.mb16 { margin-bottom: 16px; }
.mb24 { margin-bottom: 24px; }

.desktop-footer { display: flex; gap: 16px; margin-top: 8px; }
.btn-secondary {
  flex: 1; height: 50px; border: 1px solid #e2e6f0; border-radius: 12px;
  background: #ffffff; font-size: 15px; font-weight: 500; color: #6b7490; cursor: pointer;
}
.btn-secondary:hover { background: #f5f6fa; }
.btn-primary {
  flex: 2; height: 50px; background: #1a2b5e; color: #fff; border: none;
  border-radius: 12px; font-size: 15px; font-weight: 600; cursor: pointer;
}
.btn-primary:hover { background: #0f1c42; }
.contact-line { text-align: center; font-size: 12px; color: #6b7490; margin-top: 12px; }

/* ── MOBILE LAYOUT ── */
.mobile-wrap { max-width: 440px; margin: 0 auto; padding: 32px 20px 40px; display: flex; flex-direction: column; }
.page-title-mobile { font-size: 26px; font-weight: 700; color: #1a1e2e; text-align: center; margin-bottom: 28px; }
.mob-step { display: flex; flex-direction: column; gap: 20px; }
.mob-footer { margin-top: 28px; display: flex; flex-direction: column; gap: 10px; }
.mob-footer .btn-primary, .mob-footer .btn-secondary { flex: unset; width: 100%; }

/* Slide Animation */
.slide-enter-active, .slide-leave-active { transition: all 0.22s ease; }
.slide-enter-from { opacity: 0; transform: translateX(20px); }
.slide-leave-to   { opacity: 0; transform: translateX(-20px); }

/* Success screen */
.success-screen {
  padding: 60px 20px; text-align: center;
  display: flex; flex-direction: column; align-items: center; gap: 16px;
}
.success-icon {
  width: 72px; height: 72px; border-radius: 50%;
  background: #dcfce7; display: flex; align-items: center; justify-content: center;
}
.success-title { font-size: 22px; font-weight: 700; color: #1a2b5e; }
.success-sub { font-size: 14px; color: #6b7490; max-width: 380px; line-height: 1.6; }

@media (min-width: 768px) { .mobile-only { display: none !important; } }
@media (max-width: 767px) {
  .desktop-only { display: none !important; }
  .header { padding: 0 16px; }
  .g4, .g3 { grid-template-columns: 1fr 1fr; }
}
</style>