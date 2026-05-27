<template>
  <Header />
  <div class="page-container">
    <div class="controls-panel">
      <div class="search-box">
        <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="#a8a8a8" stroke-width="2">
          <circle cx="11" cy="11" r="8"></circle>
          <line x1="21" y1="21" x2="16.65" y2="16.65"></line>
        </svg>
        <input 
          type="text" 
          v-model="searchQuery" 
          placeholder="Поиск по названию или описанию..." 
          class="search-input" 
        />
      </div>
      
      <div class="filters">
        <button class="btn-outline" @click="filterOpen = true">
          <svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
            <polygon points="22 3 2 3 10 12.46 10 19 14 21 14 12.46 22 3"></polygon>
          </svg>
          Фильтры
        </button>
        <button class="btn-outline" @click="toggleSort">
          <svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
            <line x1="8" y1="6" x2="21" y2="6"/><line x1="8" y1="12" x2="21" y2="12"/><line x1="8" y1="18" x2="21" y2="18"/>
            <polyline points="3 6 3.01 6"/><polyline points="3 12 3.01 12"/><polyline points="3 18 3.01 18"/>
          </svg>
          {{ sortLabel }}
        </button>
      </div>
    </div>

    <h2 class="section-title">Рекомендуемые мероприятия</h2>
    <div class="events-grid">
      <div v-if="filteredEvents.length === 0" class="no-results">Мероприятий не найдено</div>
      <div v-for="event in filteredEvents" :key="event.id" class="event-card">
        <div class="card-image-wrap">
          <div class="card-image-placeholder">
            <svg width="48" height="48" viewBox="0 0 24 24" fill="none" stroke="#8ca0c8" stroke-width="1">
              <rect x="3" y="3" width="18" height="18" rx="2"/><path d="m3 9 4-4 4 4 4-4 4 4"/><circle cx="8.5" cy="14" r="1.5"/><path d="m21 15-5-5-5 5"/>
            </svg>
          </div>
          <span class="badge-recommended" v-if="event.recommended">Рекомендуем</span>
          <button class="btn-fav" :class="{ active: event.fav }" @click.stop="event.fav = !event.fav">
            <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="#6b7490" stroke-width="2">
              <path d="M20.84 4.61a5.5 5.5 0 0 0-7.78 0L12 5.67l-1.06-1.06a5.5 5.5 0 0 0-7.78 7.78l1.06 1.06L12 21.23l7.78-7.78 1.06-1.06a5.5 5.5 0 0 0 0-7.78z"/>
            </svg>
          </button>
        </div>
        
        <div class="card-content">
          <h3 class="card-title">{{ event.name }}</h3>
          <p class="card-audience">{{ event.desc }}</p>
          
          <div class="card-tags">
            <span 
              v-for="tag in event.tags" 
              :key="tag" 
              class="tag" 
              :class="tag.length > 8 ? 'tag-gray' : 'tag-blue'"
            >
              {{ tag }}
            </span>
          </div>
          
          <div class="card-meta">
            <div class="meta-item">
              <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><rect x="3" y="4" width="18" height="18" rx="2" ry="2"></rect><line x1="16" y1="2" x2="16" y2="6"></line><line x1="8" y1="2" x2="8" y2="6"></line><line x1="3" y1="10" x2="21" y2="10"></line></svg>
              {{ event.date }}
            </div>
            <div class="meta-item">
              <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><circle cx="12" cy="12" r="10"></circle><polyline points="12 6 12 12 16 14"></polyline></svg>
              Дедлайн: {{ event.deadline }}
            </div>
          </div>
          
          <div class="meta-location">
            <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M21 10c0 7-9 13-9 13s-9-6-9-13a9 9 0 0 1 18 0z"></path><circle cx="12" cy="10" r="3"></circle></svg>
            {{ event.city }}
          </div>
        </div>
      </div>
    </div>

    <h2 class="section-title">В вашем городе</h2>
    <div class="events-grid">
      <div v-if="filteredCityEvents.length === 0" class="no-results">В выбранном городе мероприятий не найдено</div>
      <div v-for="event in filteredCityEvents" :key="'city-' + event.id" class="event-card">
        <div class="card-image-wrap">
          <div class="card-image-placeholder">
            <svg width="48" height="48" viewBox="0 0 24 24" fill="none" stroke="#8ca0c8" stroke-width="1">
              <rect x="3" y="3" width="18" height="18" rx="2"/><path d="m3 9 4-4 4 4 4-4 4 4"/><circle cx="8.5" cy="14" r="1.5"/><path d="m21 15-5-5-5 5"/>
            </svg>
          </div>
          <button class="btn-fav" :class="{ active: event.fav }" @click.stop="event.fav = !event.fav">
            <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="#6b7490" stroke-width="2">
              <path d="M20.84 4.61a5.5 5.5 0 0 0-7.78 0L12 5.67l-1.06-1.06a5.5 5.5 0 0 0-7.78 7.78l1.06 1.06L12 21.23l7.78-7.78 1.06-1.06a5.5 5.5 0 0 0 0-7.78z"/>
            </svg>
          </button>
        </div>
        
        <div class="card-content">
          <h3 class="card-title">{{ event.name }}</h3>
          <p class="card-audience">{{ event.desc }}</p>
          
          <div class="card-tags">
            <span 
              v-for="tag in event.tags" 
              :key="tag" 
              class="tag" 
              :class="tag.length > 8 ? 'tag-gray' : 'tag-blue'"
            >
              {{ tag }}
            </span>
          </div>
          
          <div class="card-meta">
            <div class="meta-item">
              <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><rect x="3" y="4" width="18" height="18" rx="2" ry="2"></rect><line x1="16" y1="2" x2="16" y2="6"></line><line x1="8" y1="2" x2="8" y2="6"></line><line x1="3" y1="10" x2="21" y2="10"></line></svg>
              {{ event.date }}
            </div>
            <div class="meta-item">
              <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><circle cx="12" cy="12" r="10"></circle><polyline points="12 6 12 12 16 14"></polyline></svg>
              Дедлайн: {{ event.deadline }}
            </div>
          </div>
          
          <div class="meta-location">
            <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M21 10c0 7-9 13-9 13s-9-6-9-13a9 9 0 0 1 18 0z"></path><circle cx="12" cy="10" r="3"></circle></svg>
            {{ event.city }}
          </div>
        </div>
      </div>
    </div>

    <div class="overlay-bg" :class="{ visible: filterOpen }" @click="filterOpen = false"></div>
    
    <aside class="filter-panel" :class="{ open: filterOpen }">
      <div class="filter-header">
        <span class="filter-title">Фильтры</span>
        <button class="close-btn" @click="filterOpen = false">
          <svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><line x1="18" y1="6" x2="6" y2="18"/><line x1="6" y1="6" x2="18" y2="18"/></svg>
        </button>
      </div>

      <div class="filter-body">
        <div class="filter-section">
          <div class="city-row">
            <span class="filter-inline-label">Город:</span>
            <select class="city-select" v-model="filters.city">
              <option value="">Выберите город</option>
              <option v-for="c in cities" :key="c" :value="c">{{ c }}</option>
            </select>
          </div>
        </div>

        <div class="filter-section">
          <div class="filter-label">Тип мероприятия:</div>
          <div class="check-list">
            <div>
              <div class="check-item" @click="toggleType('conference')">
                <div class="check-box" :class="{ checked: filters.types.conference }">
                  <svg width="11" height="11" viewBox="0 0 24 24" fill="none" stroke="#fff" stroke-width="3"><polyline points="20 6 9 17 4 12"/></svg>
                </div>
                <span class="check-text">Конференция</span>
                <svg class="check-expand" :class="{ rotated: filters.confExpanded }" @click.stop="filters.confExpanded = !filters.confExpanded" width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><polyline points="6 9 12 15 18 9"/></svg>
              </div>
              
              <div class="expand-content" :class="{ open: filters.confExpanded }">
                <div class="sub-label">Форма участия:</div>
                <div class="check-list">
                  <div class="check-item" v-for="opt in confParticipation" :key="opt.val" @click="toggleArr(filters.confPart, opt.val)">
                    <div class="check-box" :class="{ checked: filters.confPart.includes(opt.val) }">
                      <svg width="11" height="11" viewBox="0 0 24 24" fill="none" stroke="#fff" stroke-width="3"><polyline points="20 6 9 17 4 12"/></svg>
                    </div>
                    <span class="check-text">{{ opt.label }}</span>
                  </div>
                </div>
                <div class="sub-label">Индексация:</div>
                <div class="check-list">
                  <div class="check-item" v-for="opt in indexOptions" :key="opt.val" @click="toggleArr(filters.indexing, opt.val)">
                    <div class="check-box" :class="{ checked: filters.indexing.includes(opt.val) }">
                      <svg width="11" height="11" viewBox="0 0 24 24" fill="none" stroke="#fff" stroke-width="3"><polyline points="20 6 9 17 4 12"/></svg>
                    </div>
                    <span class="check-text">{{ opt.label }}</span>
                  </div>
                </div>
              </div>
            </div>

            <div class="check-item" v-for="t in otherTypes" :key="t.val" @click="toggleArr(filters.otherTypes, t.val)">
              <div class="check-box" :class="{ checked: filters.otherTypes.includes(t.val) }">
                <svg width="11" height="11" viewBox="0 0 24 24" fill="none" stroke="#fff" stroke-width="3"><polyline points="20 6 9 17 4 12"/></svg>
              </div>
              <span class="check-text">{{ t.label }}</span>
            </div>
          </div>
        </div>

        <div class="filter-section">
          <div class="filter-label">Форма проведения:</div>
          <div class="check-list">
            <div class="check-item" v-for="f in formats" :key="f.val" @click="toggleArr(filters.formats, f.val)">
              <div class="check-box" :class="{ checked: filters.formats.includes(f.val) }">
                <svg width="11" height="11" viewBox="0 0 24 24" fill="none" stroke="#fff" stroke-width="3"><polyline points="20 6 9 17 4 12"/></svg>
              </div>
              <span class="check-text">{{ f.label }}</span>
            </div>
          </div>
        </div>

        <div class="filter-section">
          <div class="filter-label">Проводится начиная с:</div>
          <input class="date-input" type="date" v-model="filters.date">
        </div>
      </div>

      <div class="filter-footer">
        <button class="btn-reset" @click="resetFilters">Сбросить</button>
        <button class="btn-apply" @click="filterOpen = false">Применить</button>
      </div>
    </aside>
  </div>
</template>

<script setup>
import { ref, computed, reactive } from 'vue';
import Header from '../commons/Header.vue';

// Управление интерфейсом
const filterOpen = ref(false);
const searchQuery = ref('');
const sortAsc = ref(false);

const sortLabel = computed(() => sortAsc.value ? 'Название: А → Я' : 'Название: Я → А');
const toggleSort = () => { sortAsc.value = !sortAsc.value; };

// Инициализатор чистого состояния фильтров
const initFilters = () => ({
  city: '',
  types: { conference: false },
  confExpanded: false,
  confPart: [],
  indexing: [],
  otherTypes: [],
  formats: [],
  date: '',
});

const filters = reactive(initFilters());

// Константные опции для списков фильтрации
const cities = ['Москва', 'Санкт-Петербург', 'Екатеринбург', 'Новосибирск', 'Казань'];
const confParticipation = [
  { val: 'report', label: 'С докладом и публикацией' },
  { val: 'noreport', label: 'Без доклада с публикацией' },
  { val: 'listener', label: 'Слушатель' },
];
const indexOptions = [
  { val: 'rinc', label: 'РИНЦ' },
  { val: 'vak', label: 'ВАК' },
  { val: 'scopus', label: 'Scopus' },
  { val: 'wos', label: 'Web of Science' },
];
const otherTypes = [
  { val: 'contest', label: 'Конкурс' },
  { val: 'hackathon', label: 'Хакатон' },
  { val: 'case', label: 'Кейс-чемпионат' },
  { val: 'olympiad', label: 'Олимпиада' },
  { val: 'competition', label: 'Соревнование' },
  { val: 'seminar', label: 'Семинар' },
];
const formats = [
  { val: 'online', label: 'Онлайн' },
  { val: 'hybrid', label: 'Гибрид' },
  { val: 'offline', label: 'Очно' },
];

// Методы переключения состояний чекбоксов
const toggleType = (t) => { 
  filters.types[t] = !filters.types[t]; 
  if (filters.types.conference) filters.confExpanded = true;
};

const toggleArr = (arr, val) => {
  const i = arr.indexOf(val);
  if (i >= 0) arr.splice(i, 1); else arr.push(val);
};

const resetFilters = () => {
  Object.assign(filters, initFilters());
};

// Базовые массивы данных
const allEvents = ref([
  { id: 1, name: 'Международная конференция по нейросетям', type: 'conference', format: 'online', city: 'Москва', confPart: ['report'], indexing: ['wos', 'scopus'], tags: ['Конференция', 'Онлайн', 'Web of Science'], desc: 'Участие могут принять студенты бакалавриата, магистратуры, аспиранты', date: '25.01.2026', deadline: '19.04.2026', recommended: true, fav: false },
  { id: 2, name: 'Хакатон по искусственному интеллекту', type: 'hackathon', format: 'offline', city: 'Санкт-Петербург', confPart: [], indexing: [], tags: ['Хакатон', 'Очно'], desc: 'Командные соревнования по разработке AI-решений за 48 часов', date: '15.02.2026', deadline: '01.02.2026', recommended: true, fav: false },
  { id: 3, name: 'Кейс-чемпионат «Цифровая экономика»', type: 'case', format: 'hybrid', city: 'Екатеринбург', confPart: [], indexing: [], tags: ['Кейс-чемпионат', 'Гибрид'], desc: 'Участие могут принять студенты бакалавриата, магистратуры, аспиранты', date: '10.03.2026', deadline: '25.02.2026', recommended: true, fav: false },
  { id: 4, name: 'Симпозиум по квантовым вычислениям', type: 'conference', format: 'offline', city: 'Москва', confPart: ['listener'], indexing: ['vak', 'rinc'], tags: ['Конференция', 'Очно', 'ВАК'], desc: 'Доклады ведущих учёных в области квантовой физики и информатики', date: '05.04.2026', deadline: '20.03.2026', recommended: true, fav: false },
  { id: 5, name: 'Олимпиада по математике и программированию', type: 'olympiad', format: 'online', city: 'Новосибирск', confPart: [], indexing: [], tags: ['Олимпиада', 'Онлайн'], desc: 'Индивидуальные задания для студентов технических специальностей', date: '20.03.2026', deadline: '10.03.2026', recommended: false, fav: false },
  { id: 6, name: 'Семинар по методам машинного обучения', type: 'seminar', format: 'online', city: 'Казань', confPart: [], indexing: ['rinc'], tags: ['Семинар', 'Онлайн', 'РИНЦ'], desc: 'Практические занятия по современным алгоритмам ML и глубокому обучению', date: '12.04.2026', deadline: '05.04.2026', recommended: false, fav: false },
  { id: 7, name: 'Конкурс научных проектов молодых учёных', type: 'contest', format: 'hybrid', city: 'Санкт-Петербург', confPart: [], indexing: [], tags: ['Конкурс', 'Гибрид'], desc: 'Участие могут принять студенты бакалавриата, магистратуры, аспиранты', date: '30.04.2026', deadline: '15.04.2026', recommended: false, fav: false },
  { id: 8, name: 'Конференция по биоинформатике и геномике', type: 'conference', format: 'hybrid', city: 'Екатеринбург', confPart: ['report', 'noreport'], indexing: ['scopus', 'rinc'], tags: ['Конференция', 'Гибрид', 'Scopus'], desc: 'Актуальные исследования в области биоинформатики и геномного анализа', date: '18.05.2026', deadline: '01.05.2026', recommended: false, fav: false },
]);

const cityEvents = ref([
  { id: 101, name: 'Региональный конкурс студенческих работ', type: 'contest', format: 'offline', city: 'Екатеринбург', confPart: [], indexing: [], tags: ['Конкурс', 'Очно'], desc: 'Участие могут принять студенты бакалавриата, магистратуры, аспиранты', date: '25.01.2026', deadline: '19.04.2026', recommended: false, fav: false },
  { id: 102, name: 'Городской хакатон по робототехнике', type: 'hackathon', format: 'offline', city: 'Екатеринбург', confPart: [], indexing: [], tags: ['Хакатон', 'Очно'], desc: 'Командные соревнования по созданию роботизированных систем', date: '15.02.2026', deadline: '01.02.2026', recommended: false, fav: false },
  { id: 103, name: 'Уральская конференция по IT-безопасности', type: 'conference', format: 'offline', city: 'Екатеринбург', confPart: ['report'], indexing: ['vak'], tags: ['Конференция', 'Очно', 'ВАК'], desc: 'Участие могут принять студенты бакалавриата, магистратуры, аспиранты', date: '10.03.2026', deadline: '25.02.2026', recommended: false, fav: false },
  { id: 104, name: 'Олимпиада по физике УрФУ', type: 'olympiad', format: 'offline', city: 'Екатеринбург', confPart: [], indexing: [], tags: ['Олимпиада', 'Очно'], desc: 'Задания по теоретической и экспериментальной физике для студентов', date: '05.04.2026', deadline: '20.03.2026', recommended: false, fav: false },
]);

// Универсальная реактивная фильтрация
const applyEventFilters = (list, isCitySection = false) => {
  let result = [...list];

  if (searchQuery.value.trim()) {
    const query = searchQuery.value.toLowerCase().trim();
    result = result.filter(e => e.name.toLowerCase().includes(query) || e.desc.toLowerCase().includes(query));
  }

  const targetCity = filters.city || (isCitySection ? 'Екатеринбург' : '');
  if (targetCity) {
    result = result.filter(e => e.city === targetCity);
  }

  const activeTypes = [...filters.otherTypes];
  if (filters.types.conference) activeTypes.push('conference');
  if (activeTypes.length > 0) {
    result = result.filter(e => activeTypes.includes(e.type));
  }

  if (filters.formats.length > 0) {
    result = result.filter(e => filters.formats.includes(e.format));
  }

  if (filters.confPart.length > 0) {
    result = result.filter(e => e.type !== 'conference' || e.confPart.some(p => filters.confPart.includes(p)));
  }

  if (filters.indexing.length > 0) {
    result = result.filter(e => e.indexing.some(idx => filters.indexing.includes(idx)));
  }

  if (filters.date) {
    const selectedDate = new Date(filters.date);
    result = result.filter(e => {
      const [d, m, y] = e.date.split('.');
      return new Date(`${y}-${m}-${d}`) >= selectedDate;
    });
  }

  result.sort((a, b) => {
    return sortAsc.value ? a.name.localeCompare(b.name, 'ru') : b.name.localeCompare(a.name, 'ru');
  });

  return result;
};

const filteredEvents = computed(() => applyEventFilters(allEvents.value, false));
const filteredCityEvents = computed(() => applyEventFilters(cityEvents.value, true));
</script>

<style scoped lang="scss">
/* ИСПОЛЬЗУЕМ ВАШУ СТРУКТУРУ ИЗ DASHBOARD.VUE С ДОБАВЛЕНИЕМ НУЖНЫХ СТИЛЕЙ БОКОВОЙ ПАНЕЛИ */
.page-container {
    padding: 32px 40px;
    background-color: variables.$white;
    min-height: 100vh;
    position: relative;
}

.controls-panel {
    display: flex;
    justify-content: space-between;
    margin-bottom: 40px;
    gap: 16px;
}

.search-box {
    flex: 1;
    display: flex;
    align-items: center;
    background: variables.$dark-white;
    padding: 0 16px;
    border-radius: functions.radius('s');
    
    .search-input {
        width: 100%;
        padding: 16px;
        border: none;
        background: transparent;
        outline: none;
        font-size: 14px;
        color: variables.$black;
        
        &::placeholder { color: variables.$light-gray; }
    }
}

.filters {
    display: flex;
    gap: 12px;
    
    .btn-outline {
        display: flex;
        align-items: center;
        gap: 8px;
        padding: 0 20px;
        border: 1px solid variables.$ultra-light-gray;
        background: variables.$white;
        border-radius: functions.radius('s');
        color: variables.$dark-gray;
        font-size: 14px;
        cursor: pointer;
        transition: all 0.2s;
        
        &:hover {
            border-color: variables.$light-gray;
        }
        svg { flex-shrink: 0; }
    }
}

.section-title {
    font-size: 24px;
    font-weight: 700;
    color: variables.$main-color;
    margin-bottom: 24px;
    margin-top: 40px;
    &:first-of-type { margin-top: 0; }
}

.events-grid {
    display: grid;
    grid-template-columns: repeat(auto-fill, minmax(280px, 1fr));
    gap: 24px;
}

.no-results {
    grid-column: 1 / -1;
    text-align: center;
    padding: 40px;
    color: variables.$dark-gray;
    font-size: 15px;
    background: variables.$dark-white;
    border-radius: functions.radius('l');
}

.event-card {
    background: variables.$white;
    border: 1px solid variables.$dark-white;
    border-radius: functions.radius('l');
    overflow: hidden;
    box-shadow: 0 4px 12px rgba(0,0,0,0.03);
    transition: transform 0.2s;
    &:hover { transform: translateY(-2px); }
}

.card-image-wrap {
    position: relative;
    height: 180px;
    background: variables.$ultra-light-gray;
    
    .card-image-placeholder {
        width: 100%;
        height: 100%;
        display: flex;
        align-items: center;
        justify-content: center;
        background: linear-gradient(135deg, #c7d2f0 0%, #a8b4d8 100%);
    }
    
    .badge-recommended {
        position: absolute;
        top: 16px;
        right: 16px;
        background: variables.$main-color;
        color: variables.$white;
        padding: 6px 12px;
        border-radius: functions.radius('xs');
        font-size: 12px;
        font-weight: 500;
        z-index: 2;
    }

    .btn-fav {
        position: absolute;
        top: 16px;
        left: 16px;
        width: 32px;
        height: 32px;
        border-radius: 50%;
        background: rgba(255,255,255,0.9);
        border: none;
        cursor: pointer;
        display: flex;
        align-items: center;
        justify-content: center;
        z-index: 2;
        transition: background 0.15s;
        &:hover { background: #fff; }
        &.active svg { fill: #e24b4a; stroke: #e24b4a; }
    }
}

.card-content {
    padding: 24px;
}

.card-title {
    font-size: 18px;
    font-weight: 600;
    color: variables.$black;
    margin: 0 0 8px 0;
}

.card-audience {
    font-size: 13px;
    color: variables.$dark-gray;
    margin: 0 0 16px 0;
    line-height: 1.4;
}

.card-tags {
    display: flex;
    flex-wrap: wrap;
    gap: 8px;
    margin-bottom: 24px;
    
    .tag {
        padding: 6px 16px;
        border-radius: 20px;
        font-size: 11px;
        font-weight: 500;
        
        &-gray {
            background: variables.$dark-gray;
            color: variables.$white;
        }
        
        &-blue {
            background: variables.$main-color;
            color: variables.$white;
        }
    }
}

.card-meta {
    display: flex;
    gap: 24px;
    margin-bottom: 12px;
}

.meta-item, .meta-location {
    display: flex;
    align-items: center;
    gap: 8px;
    font-size: 13px;
    color: variables.$main-color;
    svg { flex-shrink: 0; }
}
.meta-location { margin-top: 6px; }

/* СТИЛИ ПАНЕЛИ ФИЛЬТРОВ ИЗ КАТАЛОГА */
.overlay-bg {
    position: fixed;
    inset: 0;
    background: rgba(10,15,40,0.35);
    z-index: 200;
    opacity: 0;
    pointer-events: none;
    transition: opacity 0.22s;
    &.visible { opacity: 1; pointer-events: all; }
}

.filter-panel {
    position: fixed;
    top: 0;
    right: 0;
    height: 100%;
    width: 360px;
    background: variables.$white;
    border-left: 1px solid variables.$dark-white;
    z-index: 201;
    display: flex;
    flex-direction: column;
    transform: translateX(100%);
    transition: transform 0.24s cubic-bezier(0.32,0,0.15,1);
    &.open { transform: translateX(0); }
}

.filter-header {
    display: flex;
    align-items: center;
    justify-content: space-between;
    padding: 22px 24px 18px;
    border-bottom: 1px solid variables.$dark-white;
}
.filter-title { font-size: 20px; font-weight: 700; color: variables.$black; }
.close-btn {
    width: 32px;
    height: 32px;
    border-radius: 50%;
    border: 1px solid variables.$ultra-light-gray;
    background: transparent;
    cursor: pointer;
    display: flex;
    align-items: center;
    justify-content: center;
    color: variables.$dark-gray;
    &:hover { background: variables.$dark-white; }
}

.filter-body { flex: 1; overflow-y: auto; padding: 20px 24px; }
.filter-section { margin-bottom: 24px; }
.filter-label {
    font-size: 12px;
    font-weight: 600;
    color: variables.$dark-gray;
    margin-bottom: 10px;
    text-transform: uppercase;
}

.city-row { display: flex; align-items: center; gap: 12px; }
.filter-inline-label { font-size: 14px; color: variables.$black; }
.city-select {
    flex: 1;
    height: 42px;
    border: 1px solid variables.$ultra-light-gray;
    border-radius: functions.radius('s');
    background: variables.$dark-white;
    padding: 0 14px;
    outline: none;
}

.check-list { display: flex; flex-direction: column; }
.check-item {
    display: flex;
    align-items: center;
    padding: 8px 0;
    cursor: pointer;
    user-select: none;
    &:hover { background: variables.$dark-white; border-radius: functions.radius('s'); padding-left: 6px; padding-right: 6px; margin: 0 -6px; }
}
.check-box {
    width: 18px;
    height: 18px;
    border-radius: 4px;
    border: 1.5px solid variables.$light-gray;
    background: variables.$white;
    display: flex;
    align-items: center;
    justify-content: center;
    margin-right: 10px;
    svg { display: none; }
    &.checked {
        background: variables.$main-color;
        border-color: variables.$main-color;
        svg { display: block; }
    }
}
.check-text { font-size: 14px; color: variables.$black; flex: 1; }
.check-expand { color: variables.$dark-gray; transition: transform 0.18s; &.rotated { transform: rotate(180deg); } }

.expand-content {
    max-height: 0;
    overflow: hidden;
    transition: max-height 0.25s ease;
    padding-left: 28px;
    &.open { max-height: 400px; }
}
.sub-label { font-size: 11px; font-weight: 600; color: variables.$dark-gray; margin: 10px 0 6px; text-transform: uppercase; }

.date-input {
    width: 100%;
    height: 42px;
    border: 1px solid variables.$ultra-light-gray;
    border-radius: functions.radius('s');
    background: variables.$dark-white;
    padding: 0 14px;
    outline: none;
}

.filter-footer {
    padding: 16px 24px;
    border-top: 1px solid variables.$dark-white;
    display: flex;
    gap: 12px;
}
.btn-reset {
    flex: 1;
    height: 46px;
    border: 1px solid variables.$ultra-light-gray;
    border-radius: functions.radius('s');
    background: transparent;
    color: variables.$dark-gray;
    cursor: pointer;
}
.btn-apply {
    flex: 1.6;
    height: 46px;
    border: none;
    border-radius: functions.radius('s');
    background: variables.$main-color;
    color: variables.$white;
    font-weight: 600;
    cursor: pointer;
}

@media (max-width: 520px) {
    .filter-panel { width: 100%; }
}
</style>