<template>
  <div class="page-container">
    <Header />
    <main class="main-content">
      
      <div v-if="page === 'list'">
        <div class="page-header">
          <h1 class="page-title">Мои мероприятия</h1>
          <button class="btn-primary" @click="startWizard">Создать</button>
        </div>

        <div class="list-container">
          <div v-for="ev in myEvents" :key="ev.id" class="list-row">
            <div class="row-image">
              <svg width="32" height="32" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.2"><rect x="3" y="3" width="18" height="18" rx="2"/><path d="m3 9 4-4 4 4 4-4 4 4"/><circle cx="8.5" cy="14.5" r="1.5"/><path d="m21 15-5-5-5 5"/></svg>
            </div>
            
            <div class="row-content">
              <h3 class="row-title">{{ ev.name }}</h3>
              
              <div class="row-meta">
                <div class="meta-item">
                  <svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><rect x="3" y="4" width="18" height="18" rx="2" ry="2"></rect><line x1="16" y1="2" x2="16" y2="6"></line><line x1="8" y1="2" x2="8" y2="6"></line><line x1="3" y1="10" x2="21" y2="10"></line></svg>
                  {{ ev.date }}
                </div>
                <div class="meta-item">
                  <svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><circle cx="12" cy="12" r="10"></circle><polyline points="12 6 12 12 16 14"></polyline></svg>
                  {{ ev.time }}
                </div>
                <div class="meta-item location">
                  <svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M21 10c0 7-9 13-9 13s-9-6-9-13a9 9 0 0 1 18 0z"></path><circle cx="12" cy="10" r="3"></circle></svg>
                  {{ ev.location }}
                </div>
              </div>

              <div class="row-status-bar">
                <div class="status-group">
                  <span class="label">Тип мероприятия:</span>
                  <span class="pill pill-dark-blue" @click.stop="cycleType(ev)">
                    {{ ev.type }} ⌄
                  </span>
                </div>
                <div class="status-group">
                  <span class="label">Форма участия:</span>
                  <span class="pill pill-light-blue" @click.stop="cycleFormat(ev)">
                    {{ ev.format }} ⌄
                  </span>
                </div>
                <div class="status-group">
                  <span class="label">Модерация:</span>
                  <span class="pill pill-gray">{{ ev.moderation }}</span>
                </div>
                <div class="status-group">
                  <span class="label">Статус мероприятия:</span>
                  <span class="pill pill-dark-blue" @click.stop="cycleStatus(ev)">
                    {{ ev.status }} ⌄
                  </span>
                </div>
              </div>
            </div>
          </div>
        </div>
      </div>

      <div class="wizard-wrap" v-if="page === 'wizard'">
        <h1 class="wizard-title">Создание мероприятия</h1>

        <div class="steps-bar">
          <div v-for="(s, i) in steps" :key="i" class="step-item">
            <div class="step-col">
              <div class="step-circle" :class="{ active: wizardStep === i, done: wizardStep > i }">
                <svg v-if="wizardStep > i" width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="3"><polyline points="20 6 9 17 4 12"/></svg>
                <span v-else>{{ i + 1 }}</span>
              </div>
              <span class="step-label">{{ s }}</span>
            </div>
            <div v-if="i < steps.length - 1" class="step-line" :class="{ done: wizardStep > i }"></div>
          </div>
        </div>

        <div class="form-card">
            
          <div v-if="wizardStep === 0">
            <div class="form-step-title">Основная информация</div>
            <div class="field-group full-width">
              <div class="field">
                <label class="field-label">Название мероприятия <span class="req">*</span></label>
                <input class="field-input" :class="{ error: err.name }" v-model="form.name" placeholder="Введите название мероприятия">
                <span class="err-text" v-if="err.name">Обязательное поле</span>
              </div>
            </div>
            <div class="field-group cols-3">
              <div class="field">
                <label class="field-label">Тип мероприятия <span class="req">*</span></label>
                <select class="field-select" :class="{ error: err.type }" v-model="form.type">
                  <option value="">Не выбрано</option>
                  <option v-for="t in eventTypes" :key="t" :value="t">{{ t }}</option>
                </select>
                <span class="err-text" v-if="err.type">Обязательное поле</span>
              </div>
              <div class="field">
                <label class="field-label">Формат участия <span class="req">*</span></label>
                <select class="field-select" :class="{ error: err.format }" v-model="form.format">
                  <option value="">Не выбрано</option>
                  <option v-for="f in formats" :key="f" :value="f">{{ f }}</option>
                </select>
                <span class="err-text" v-if="err.format">Обязательное поле</span>
              </div>
              <div class="field">
                <label class="field-label">Индексация <span class="req">*</span></label>
                <select class="field-select" :class="{ error: err.indexing }" v-model="form.indexing">
                  <option value="">Не выбрано</option>
                  <option v-for="idx in indexingOptions" :key="idx" :value="idx">{{ idx }}</option>
                </select>
                <span class="err-text" v-if="err.indexing">Обязательное поле</span>
              </div>
            </div>
            <div class="field-group full-width" v-if="form.type === 'Конференция'">
              <div class="field">
                <label class="field-label">Форма участия в конференции</label>
                <select class="field-select" v-model="form.confPart">
                  <option value="">Не выбрано</option>
                  <option>С докладом и публикацией</option>
                  <option>Без доклада с публикацией</option>
                  <option>Слушатель</option>
                </select>
              </div>
            </div>
          </div>

          <div v-if="wizardStep === 1">
            <div class="form-step-title">Место и время проведения</div>
            <div class="field-group cols-2">
              <div class="field">
                <label class="field-label">Город <span class="req">*</span></label>
                <select class="field-select" :class="{ error: err.city }" v-model="form.city">
                  <option value="">Выберите город</option>
                  <option v-for="c in cities" :key="c" :value="c">{{ c }}</option>
                </select>
                <span class="err-text" v-if="err.city">Обязательное поле</span>
              </div>
              <div class="field">
                <label class="field-label">Адрес <span class="req">*</span></label>
                <input class="field-input" :class="{ error: err.address }" v-model="form.address" placeholder="Введите адрес">
                <span class="err-text" v-if="err.address">Обязательное поле</span>
              </div>
            </div>
            <div class="field-group cols-4">
              <div class="field">
                <label class="field-label">Дата начала <span class="req">*</span></label>
                <input class="field-input" :class="{ error: err.dateStart }" v-model="form.dateStart" type="date">
                <span class="err-text" v-if="err.dateStart">Обязательное поле</span>
              </div>
              <div class="field">
                <label class="field-label">Время начала <span class="req">*</span></label>
                <input class="field-input" v-model="form.timeStart" type="time">
              </div>
              <div class="field">
                <label class="field-label">Дата окончания <span class="req">*</span></label>
                <input class="field-input" :class="{ error: err.dateEnd }" v-model="form.dateEnd" type="date">
                <span class="err-text" v-if="err.dateEnd">Обязательное поле</span>
              </div>
              <div class="field">
                <label class="field-label">Время окончания <span class="req">*</span></label>
                <input class="field-input" v-model="form.timeEnd" type="time">
              </div>
            </div>
            <div class="divider-section">
              <div class="field-group cols-2">
                <div class="field">
                  <label class="field-label">Дедлайн приёма заявок <span class="req">*</span></label>
                  <input class="field-input" :class="{ error: err.deadlineDate }" v-model="form.deadlineDate" type="date">
                  <span class="err-text" v-if="err.deadlineDate">Обязательное поле</span>
                </div>
                <div class="field">
                  <label class="field-label">Время дедлайна</label>
                  <input class="field-input" v-model="form.deadlineTime" type="time">
                </div>
              </div>
            </div>
          </div>

          <div v-if="wizardStep === 2">
            <div class="form-step-title">Описание и требования</div>
            <div class="field mb-24">
              <label class="field-label">Описание <span class="req">*</span></label>
              <textarea class="field-textarea" :class="{ error: err.desc }" v-model="form.desc" placeholder="Описание вашего мероприятия"></textarea>
              <span class="err-text" v-if="err.desc">Обязательное поле</span>
            </div>

            <div class="req-section-header">
              <span class="req-section-title">Требования</span>
              <button class="btn-add-req" @click="addRequirement">
                <svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.5"><line x1="12" y1="5" x2="12" y2="19"/><line x1="5" y1="12" x2="19" y2="12"/></svg>
                Добавить
              </button>
            </div>

            <div class="req-item" v-for="(r, i) in form.requirements" :key="i">
              <div class="req-item-header">
                <span class="req-item-num">Требование {{ i + 1 }}</span>
                <button class="btn-remove" @click="removeRequirement(i)">
                  <svg width="13" height="13" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.5"><line x1="18" y1="6" x2="6" y2="18"/><line x1="6" y1="6" x2="18" y2="18"/></svg>
                  Удалить
                </button>
              </div>
              <div class="req-item-grid">
                <div class="field">
                  <label class="field-label">Тип <span class="req">*</span></label>
                  <span class="hint-field">Например: требования к оформлению тезисов</span>
                  <input class="field-input" v-model="r.name" placeholder="Наименование">
                </div>
                <div class="field">
                  <label class="field-label">Описание <span class="req">*</span></label>
                  <span class="hint-field">Текст или описание файла</span>
                  <input class="field-input" v-model="r.text" placeholder="Введите текст требований">
                </div>
              </div>
            </div>

            <div v-if="form.requirements.length === 0" class="empty-req-box">
              Нажмите «Добавить», чтобы указать требования к участникам
            </div>

            <p class="hint-bottom">
              Другие поля, например: программа мероприятия, будут доступны после прохождения модерации
            </p>
          </div>

          <div v-if="wizardStep === 3">
            <div class="form-step-title">Проверьте данные перед отправкой</div>

            <div class="review-section">
              <div class="review-section-title">Основная информация</div>
              <div class="review-grid">
                <div class="review-field">
                  <div class="review-field-label">Название</div>
                  <div class="review-field-val" :class="{ empty: !form.name }">{{ form.name || '—' }}</div>
                </div>
                <div class="review-field">
                  <div class="review-field-label">Тип мероприятия</div>
                  <div class="review-field-val" :class="{ empty: !form.type }">{{ form.type || '—' }}</div>
                </div>
                <div class="review-field">
                  <div class="review-field-label">Формат участия</div>
                  <div class="review-field-val" :class="{ empty: !form.format }">{{ form.format || '—' }}</div>
                </div>
                <div class="review-field">
                  <div class="review-field-label">Индексация</div>
                  <div class="review-field-val" :class="{ empty: !form.indexing }">{{ form.indexing || '—' }}</div>
                </div>
              </div>
            </div>

            <div class="review-section">
              <div class="review-section-title">Место и время</div>
              <div class="review-grid">
                <div class="review-field">
                  <div class="review-field-label">Город</div>
                  <div class="review-field-val" :class="{ empty: !form.city }">{{ form.city || '—' }}</div>
                </div>
                <div class="review-field">
                  <div class="review-field-label">Адрес</div>
                  <div class="review-field-val" :class="{ empty: !form.address }">{{ form.address || '—' }}</div>
                </div>
                <div class="review-field">
                  <div class="review-field-label">Дата начала</div>
                  <div class="review-field-val" :class="{ empty: !form.dateStart }">{{ formatDate(form.dateStart) || '—' }}</div>
                </div>
                <div class="review-field">
                  <div class="review-field-label">Дата окончания</div>
                  <div class="review-field-val" :class="{ empty: !form.dateEnd }">{{ formatDate(form.dateEnd) || '—' }}</div>
                </div>
                <div class="review-field">
                  <div class="review-field-label">Дедлайн заявок</div>
                  <div class="review-field-val" :class="{ empty: !form.deadlineDate }">{{ formatDate(form.deadlineDate) || '—' }}</div>
                </div>
              </div>
            </div>

            <div class="review-section">
              <div class="review-section-title">Описание</div>
              <div class="review-field">
                <div class="review-field-val" :class="{ empty: !form.desc }" style="font-weight: 400; line-height: 1.6;">{{ form.desc || 'Не заполнено' }}</div>
              </div>
            </div>

            <div class="review-section" v-if="form.requirements.length > 0">
              <div class="review-section-title">Требования ({{ form.requirements.length }})</div>
              <div v-for="(r, i) in form.requirements" :key="i" class="review-field">
                <div class="review-field-label">{{ r.name || 'Без названия' }}</div>
                <div class="review-field-val" style="font-weight: 400">{{ r.text || '—' }}</div>
              </div>
            </div>
          </div>

          <div v-if="wizardStep === 4" class="success-screen">
            <div class="success-icon">
              <svg width="32" height="32" viewBox="0 0 24 24" fill="none" stroke="#22c55e" stroke-width="2.5"><polyline points="20 6 9 17 4 12"/></svg>
            </div>
            <div class="success-title">Мероприятие отправлено на модерацию</div>
            <div class="success-sub">Другие поля (программа, спикеры, партнёры) станут доступны после прохождения модерации. Следите за статусом в разделе «Мои мероприятия».</div>
            <button class="btn-primary" style="margin-top: 8px" @click="page = 'list'">Перейти к мероприятиям</button>
          </div>

        </div>

        <div class="wizard-footer" v-if="wizardStep < 4">
          <button class="btn-back" @click="prevStep">
            {{ wizardStep === 0 ? 'Отмена' : '← Назад' }}
          </button>
          <span class="step-hint">{{ wizardStep + 1 }} из {{ steps.length }}</span>
          <button class="btn-next" @click="nextStep">
            {{ wizardStep === steps.length - 1 ? 'Отправить' : 'Далее' }}
            <svg v-if="wizardStep < steps.length - 1" width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.5"><line x1="5" y1="12" x2="19" y2="12"/><polyline points="12 5 19 12 12 19"/></svg>
            <svg v-else width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.5"><polyline points="20 6 9 17 4 12"/></svg>
          </button>
        </div>

        <div class="wizard-contact" v-if="wizardStep < 4">
          По всем вопросам можете обращаться: <a href="mailto:admin@example.com">admin@example.com</a>
        </div>
      </div>

    </main>
  </div>
</template>

<script setup>
import { ref, reactive } from 'vue';
import Header from '../commons/Header.vue';

const page = ref('list');
const wizardStep = ref(0);

const steps = ['Основная информация', 'Место и время', 'Описание и требования', 'Подтверждение'];

const eventTypes = ['Конференция', 'Конкурс', 'Хакатон', 'Кейс-чемпионат', 'Олимпиада', 'Соревнование', 'Семинар'];
const formats = ['Онлайн', 'Гибрид', 'Очно'];
const indexingOptions = ['РИНЦ', 'ВАК', 'Scopus', 'Web of Science'];
const cities = ['Москва', 'Санкт-Петербург', 'Екатеринбург', 'Новосибирск', 'Казань'];

const typesList = ['Конференция', 'Конкурс', 'Хакатон', 'Семинар'];
const formatsList = ['Онлайн', 'Гибрид', 'Очно'];
const statusList = ['В процессе', 'Завершено', 'Отменено'];

// Наполняем начальный список тестовыми данными из вашей логики организатора
const myEvents = ref([
  { id: 1, name: 'Международная конференция по нейросетям', date: '25.01.2026', time: '09:00–16:00', location: 'Екатеринбург, улица Пушкина, дом Колотушкина', type: 'Конференция', format: 'Онлайн', moderation: 'Отправлено на модерацию', status: 'В процессе', _ti: 0, _fi: 0, _si: 0 },
  { id: 2, name: 'Хакатон по искусственному интеллекту', date: '15.02.2026', time: '10:00–20:00', location: 'Санкт-Петербург, Университетская набережная, 7', type: 'Хакатон', format: 'Очно', moderation: 'Отправлено на модерацию', status: 'В процессе', _ti: 2, _fi: 2, _si: 0 },
  { id: 3, name: 'Кейс-чемпионат «Цифровая экономика»', date: '10.03.2026', time: '09:00–18:00', location: 'Москва, Ленинские горы, 1', type: 'Конкурс', format: 'Гибрид', moderation: 'Отправлено на модерацию', status: 'В процессе', _ti: 1, _fi: 1, _si: 0 },
  { id: 4, name: 'Региональный семинар по квантовым вычислениям', date: '05.04.2026', time: '11:00–15:00', location: 'Новосибирск, Академгородок, проспект Науки, 9', type: 'Семинар', format: 'Онлайн', moderation: 'Отправлено на модерацию', status: 'В процессе', _ti: 3, _fi: 0, _si: 0 },
]);

// Интерактивное зацикленное изменение полей по клику (перекочевало из organizer.html)
const cycleType = (ev) => { ev._ti = (ev._ti + 1) % typesList.length; ev.type = typesList[ev._ti]; };
const cycleFormat = (ev) => { ev._fi = (ev._fi + 1) % formatsList.length; ev.format = formatsList[ev._fi]; };
const cycleStatus = (ev) => { ev._si = (ev._si + 1) % statusList.length; ev.status = statusList[ev._si]; };

const form = reactive({
  name: '', type: '', format: '', indexing: '', confPart: '',
  city: '', address: '', dateStart: '', timeStart: '09:00', dateEnd: '', timeEnd: '17:00',
  deadlineDate: '', deadlineTime: '23:59',
  desc: '',
  requirements: [],
});

const err = reactive({
  name: false, type: false, format: false, indexing: false,
  city: false, address: false, dateStart: false, dateEnd: false, deadlineDate: false,
  desc: false,
});

const clearErr = () => Object.keys(err).forEach(k => err[k] = false);

const validateStep = (step) => {
  clearErr();
  let ok = true;
  if (step === 0) {
    if (!form.name.trim()) { err.name = true; ok = false; }
    if (!form.type) { err.type = true; ok = false; }
    if (!form.format) { err.format = true; ok = false; }
    if (!form.indexing) { err.indexing = true; ok = false; }
  }
  if (step === 1) {
    if (!form.city) { err.city = true; ok = false; }
    if (!form.address.trim()) { err.address = true; ok = false; }
    if (!form.dateStart) { err.dateStart = true; ok = false; }
    if (!form.dateEnd) { err.dateEnd = true; ok = false; }
    if (!form.deadlineDate) { err.deadlineDate = true; ok = false; }
  }
  if (step === 2) {
    if (!form.desc.trim()) { err.desc = true; ok = false; }
  }
  return ok;
};

const nextStep = () => {
  if (!validateStep(wizardStep.value)) return;
  wizardStep.value++;
  if (wizardStep.value === 4) {
    const fmtD = (d) => d ? d.split('-').reverse().join('.') : '';
    myEvents.value.unshift({
      id: Date.now(),
      name: form.name,
      date: fmtD(form.dateStart),
      time: `${form.timeStart}–${form.timeEnd}`,
      location: `${form.city}, ${form.address}`,
      type: form.type,
      format: form.format,
      moderation: 'Отправлено на модерацию',
      status: 'В процессе',
      _ti: 0, _fi: 0, _si: 0,
    });
  }
};

const prevStep = () => {
  if (wizardStep.value === 0) { page.value = 'list'; return; }
  clearErr();
  wizardStep.value--;
};

const addRequirement = () => form.requirements.push({ name: '', text: '' });
const removeRequirement = (i) => form.requirements.splice(i, 1);

const formatDate = (d) => d ? d.split('-').reverse().join('.') : '';

const startWizard = () => {
  Object.assign(form, {
    name: '', type: '', format: '', indexing: '', confPart: '',
    city: '', address: '', dateStart: '', timeStart: '09:00', dateEnd: '', timeEnd: '17:00',
    deadlineDate: '', deadlineTime: '23:59', desc: '', requirements: [],
  });
  clearErr();
  wizardStep.value = 0;
  page.value = 'wizard';
};
</script>

<style lang="scss" scoped>

.page-container {
  padding: 40px;
  background-color: variables.$white;
  min-height: 100vh;
}

/* ── HEADER ── */
.header {
  background: variables.$white;
  border-bottom: 1px solid #e2e6f0; // В соответствии с дизайном organizer.html
  padding: 0 0 16px 0;
  margin-bottom: 32px;
  display: flex;
  align-items: center;
  justify-content: space-between;

  &-left { display: flex; align-items: center; gap: 36px; }
  
  .logo { 
    font-size: 20px; 
    font-weight: 700; 
    color: #1a2b5e; 
    letter-spacing: -0.5px; 
    cursor: pointer; 
  }
  
  .nav { 
    display: flex; 
    gap: 28px; 
    
    &-link {
      font-size: 14px; 
      font-weight: 500; 
      color: variables.$gray;
      cursor: pointer; 
      padding-bottom: 2px;
      border-bottom: 2px solid transparent; 
      transition: color 0.15s, border-color 0.15s;

      &.active { 
        color: #1a2b5e; 
        border-bottom-color: #1d4ed8; 
      }
    }
  }

  &-right { display: flex; align-items: center; gap: 12px; }

  .icon-btn {
    width: 38px; height: 38px; border-radius: 50%;
    border: 1px solid #e2e6f0; background: transparent;
    cursor: pointer; display: flex; align-items: center; justify-content: center;
    color: variables.$gray; position: relative; transition: background 0.15s;

    &:hover { background: variables.$dark-white; }
  }

  .notif-badge {
    position: absolute; top: -3px; right: -3px;
    background: #1d4ed8; color: #fff;
    font-size: 10px; font-weight: 600; border-radius: 10px;
    padding: 1px 5px; min-width: 18px; text-align: center; line-height: 16px;
  }
}

/* ── MAIN CONTENT WRAPPER ── */
.main-content {
  max-width: 1360px;
  margin: 0 auto;
}

/* ── СТРАНИЦА СПИСКА (Page Header) ── */
.page-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-bottom: 32px;
}

.page-title {
  font-size: 28px;
  color: variables.$light-black;
  margin: 0;
}

.btn-primary {
  background: #1a2b5e; // Брендовый navy из разметки
  color: variables.$white;
  padding: 12px 24px;
  border: none;
  border-radius: functions.radius('s');
  font-size: 14px;
  font-weight: 600;
  cursor: pointer;
  height: 44px;
  transition: opacity 0.2s;

  &:hover { opacity: 0.9; }
}

/* ── КОНТЕЙНЕР СПИСКА (List Container) ── */
.list-container {
  display: flex;
  flex-direction: column;
  gap: 16px;
}

.list-row {
  display: flex;
  gap: 24px;
  padding: 24px;
  background: variables.$dark-white;
  border-radius: functions.radius('l');
  border: 1px solid #e2e6f0;
  transition: box-shadow 0.15s;

  &:hover {
    box-shadow: 0 4px 16px rgba(26, 43, 94, 0.06);
  }
}

.row-image {
  width: 120px;
  height: 120px;
  background: linear-gradient(135deg, #d0d8f0, #b8c4e0);
  border-radius: functions.radius('s');
  flex-shrink: 0;
  display: flex;
  align-items: center;
  justify-content: center;
  color: #8898c8;
}

.row-content {
  flex: 1;
  display: flex;
  flex-direction: column;
  justify-content: space-between;
}

.row-title {
  font-size: 20px;
  color: variables.$light-black;
  margin: 0 0 16px 0;
  font-weight: 600;
}

.row-meta {
  display: flex;
  gap: 24px;
  margin-bottom: 24px;
  color: variables.$gray;

  .meta-item {
    display: flex;
    align-items: center;
    gap: 8px;
    font-size: 14px;
  }
}

/* ── СТАТУС-БАР И ПИЛЮЛИ (Status Bar / Pills) ── */
.row-status-bar {
  display: flex;
  align-items: center;
  gap: 24px;
  flex-wrap: wrap;

  .status-group {
    display: flex;
    align-items: center;
    gap: 8px;

    .label {
      font-size: 13px;
      color: variables.$gray;
    }

    .pill {
      padding: 6px 16px;
      border-radius: 20px;
      font-size: 13px;
      font-weight: 500;
      cursor: pointer;
      border: none;
      display: inline-flex;
      align-items: center;
      transition: opacity 0.15s;

      &:hover { opacity: 0.85; }

      &-light-blue { background: #3b82f6; color: variables.$white; }
      &-dark-blue { background: #1a2b5e; color: variables.$white; }
      &-gray { background: #e0e4f0; color: #4a5278; pointer-events: none; cursor: default; }
    }
  }
}

/* ── ВНУТРЕННИЕ СТИЛИ МАСТЕРА (WIZARD) ── */
.wizard-wrap { 
  max-width: 900px; 
  margin: 0 auto; 
}
.wizard-title { 
  font-size: 26px; 
  font-weight: 700; 
  color: #1a2b5e; 
  margin-bottom: 28px; 
}

/* Индикатор шагов */
.steps-bar { 
  display: flex; 
  align-items: center; 
  margin-bottom: 36px; 
}
.step-item { 
  display: flex; 
  align-items: center; 
  flex: 1; 
  &:last-child { flex: 0; }
}
.step-col { 
  display: flex; 
  flex-direction: column; 
  align-items: center; 
  position: relative; 
}
.step-circle {
  width: 36px; height: 36px; border-radius: 50%; flex-shrink: 0;
  display: flex; align-items: center; justify-content: center;
  font-size: 13px; font-weight: 700;
  border: 2px solid #e2e6f0; background: variables.$white; color: variables.$gray;
  transition: all 0.2s;

  &.active { border-color: #1d4ed8; background: #1d4ed8; color: #fff; }
  &.done { border-color: #1a2b5e; background: #1a2b5e; color: #fff; }
}
.step-line { 
  flex: 1; height: 2px; background: #e2e6f0; margin: 0 8px; transition: background 0.2s; 
  &.done { background: #1a2b5e; }
}
.step-label { 
  font-size: 11px; font-weight: 500; color: variables.$gray; position: absolute; white-space: nowrap; top: 42px; 
}

/* Карточка формы */
.form-card {
  background: variables.$white; border: 1px solid #e2e6f0;
  border-radius: functions.radius('l'); padding: 32px 36px;
}
.form-step-title {
  font-size: 18px; font-weight: 700; color: #1a2b5e;
  margin-bottom: 24px; padding-bottom: 16px; border-bottom: 1px solid #e2e6f0;
}

/* Сетки полей */
.field-group { 
  display: grid; gap: 20px; margin-bottom: 20px; 
  &.full-width { grid-template-columns: 1fr; }
  &.cols-2 { grid-template-columns: 1fr 1fr; }
  &.cols-3 { grid-template-columns: 1fr 1fr 1fr; }
  &.cols-4 { grid-template-columns: 1fr 1fr 1fr 1fr; }
}

.field { 
  display: flex; flex-direction: column; gap: 6px; 
}
.field-label { 
  font-size: 13px; font-weight: 500; color: variables.$light-black; 
}
.req { color: #e24b4a; }

.field-input, .field-select, .field-textarea {
  height: 42px; padding: 0 14px;
  border: 1px solid #e2e6f0; border-radius: functions.radius('s');
  background: variables.$dark-white; font-size: 14px; color: variables.$light-black; outline: none;
  transition: border-color 0.15s;

  &:focus { border-color: #1d4ed8; background: #fff; }
  &.error { border-color: #e24b4a; }
}
.field-textarea { height: 100px; padding: 12px 14px; resize: vertical; line-height: 1.5; }
.err-text { font-size: 12px; color: #e24b4a; }

.mb-24 { margin-bottom: 24px; }
.divider-section { border-top: 1px solid #e2e6f0; padding-top: 20px; margin-top: 4px; }
.hint-field { font-size: 11px; color: variables.$gray; margin-top: -2px; margin-bottom: 4px; }
.hint-bottom { margin-top: 20px; font-size: 12px; color: variables.$gray; text-align: center; }

/* Требования (Загрузчик/Конструктор) */
.req-section-header { display: flex; align-items: center; justify-content: space-between; margin-bottom: 16px; }
.req-section-title { font-size: 16px; font-weight: 700; color: #1a2b5e; }
.btn-add-req { display: flex; align-items: center; gap: 6px; color: #1d4ed8; font-size: 14px; font-weight: 600; background: none; border: none; cursor: pointer; }
.req-item { border: 1px solid #e2e6f0; border-radius: functions.radius('m'); padding: 20px; margin-bottom: 12px; background: variables.$dark-white; }
.req-item-header { display: flex; align-items: center; justify-content: space-between; margin-bottom: 14px; }
.req-item-num { font-size: 13px; font-weight: 600; color: variables.$gray; }
.btn-remove { background: none; border: none; cursor: pointer; color: #e24b4a; font-size: 13px; font-weight: 500; display: flex; align-items: center; gap: 4px; }
.req-item-grid { display: grid; grid-template-columns: 1fr 1fr; gap: 14px; }
.empty-req-box { padding: 20px; text-align: center; color: variables.$gray; font-size: 13px; border: 1px dashed #e2e6f0; border-radius: functions.radius('m'); }

/* Прeвью */
.review-section { margin-bottom: 24px; }
.review-section-title { font-size: 13px; font-weight: 700; color: variables.$gray; text-transform: uppercase; letter-spacing: 0.05em; margin-bottom: 12px; }
.review-grid { display: grid; grid-template-columns: 1fr 1fr; gap: 10px 24px; }
.review-field { padding: 10px 0; border-bottom: 1px solid #e2e6f0; }
.review-field-label { font-size: 12px; color: variables.$gray; margin-bottom: 3px; }
.review-field-val { 
  font-size: 14px; font-weight: 500; color: variables.$light-black; 
  &.empty { color: variables.$gray; font-style: italic; font-weight: 400; }
}

/* Подвал мастера */
.wizard-footer { display: flex; align-items: center; justify-content: space-between; margin-top: 28px; }
.btn-back { height: 46px; padding: 0 28px; border: 1px solid #e2e6f0; border-radius: functions.radius('m'); background: variables.$white; font-size: 15px; font-weight: 500; color: variables.$gray; cursor: pointer; }
.btn-next { 
  height: 46px; padding: 0 36px; background: #1a2b5e; color: #fff; border: none; border-radius: functions.radius('m'); font-size: 15px; font-weight: 600; cursor: pointer; display: flex; align-items: center; gap: 8px; 
  &:hover { background: #0f1c42; }
}
.step-hint { font-size: 13px; color: variables.$gray; }
.wizard-contact { text-align: center; margin-top: 20px; font-size: 12px; color: variables.$gray; a { color: #1d4ed8; text-decoration: none; } }

/* Успешная отправка */
.success-screen { text-align: center; padding: 60px 40px; display: flex; flex-direction: column; align-items: center; gap: 16px; }
.success-icon { width: 72px; height: 72px; border-radius: 50%; background: #e0f2ea; display: flex; align-items: center; justify-content: center; }
.success-title { font-size: 22px; font-weight: 700; color: #1a2b5e; }
.success-sub { font-size: 14px; color: variables.$gray; max-width: 420px; }

/* Адаптив */
@media (max-width: 860px) {
  .page-container { padding: 20px; }
  .header { padding: 0; gap: 12px; }
  .list-row { flex-direction: column; gap: 16px; padding: 16px; }
  .field-group.cols-2, .field-group.cols-3, .field-group.cols-4 { grid-template-columns: 1fr; }
  .req-item-grid, .review-grid { grid-template-columns: 1fr; }
  .form-card { padding: 20px; }
}
</style>