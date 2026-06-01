<template>
  <div class="form-wrapper">
    <div class="form-container">
      <header class="form-header">
        <button class="back-button" aria-label="Назад" @click="$router.back()">
          <svg width="10" height="16" viewBox="0 0 10 16" fill="none">
            <path d="M8.5 15L1.5 8L8.5 1" stroke="#7a7a7a" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"/>
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
            v-model="formData.first_name" 
            placeholder="Олег"
            :class="{ 'input-error': errors.first_name }"
          >
          <span v-if="errors.first_name" class="error-text">{{ errors.first_name }}</span>
        </div>

        <div class="form-group">
          <label for="lastName">Введите вашу фамилию: <span class="required">*</span></label>
          <input 
            id="lastName" 
            type="text" 
            v-model="formData.last_name" 
            placeholder="Волков"
            :class="{ 'input-error': errors.last_name }"
          >
          <span v-if="errors.last_name" class="error-text">{{ errors.last_name }}</span>
        </div>

        <div class="form-group">
          <label for="middleName">Введите ваше отчество:</label>
          <input 
            id="middleName" 
            type="text" 
            v-model="formData.middle_name" 
            placeholder="Васильевич"
          >
        </div>

        <div class="form-group">
          <label>Ваша роль: <span class="required">*</span></label>
          <div class="slider-selector" :style="{ '--active-index': formData.role === 'organizer' ? 1 : 0 }">
            <div class="slider-active-bg"></div>
            <button 
              type="button" 
              class="slider-btn"
              :class="{ 'active': formData.role === 'participant' }"
              @click="formData.role = 'participant'"
            >Участник</button>
            <button 
              type="button" 
              class="slider-btn"
              :class="{ 'active': formData.role === 'organizer' }"
              @click="formData.role = 'organizer'"
            >Организатор</button>
          </div>
        </div>

        <div class="form-group">
          <label>Ваш пол: <span class="required">*</span></label>
          <div class="slider-selector" :style="{ '--active-index': formData.gender === 'male' ? 1 : 0 }">
            <div class="slider-active-bg"></div>
            <button 
              type="button" 
              class="slider-btn"
              :class="{ 'active': formData.gender === 'female' }"
              @click="formData.gender = 'female'"
            >Женский</button>
            <button 
              type="button" 
              class="slider-btn"
              :class="{ 'active': formData.gender === 'male' }"
              @click="formData.gender = 'male'"
            >Мужской</button>
          </div>
        </div>

        <div class="form-group" ref="universityContainer">
          <label for="university">Университет / Организация:</label>
          <div class="autocomplete-wrapper">
            <input 
              id="university" 
              type="text"
              v-model="universitySearch" 
              @input="onUniversityInput"
              @focus="isDropdownOpen = true"
              placeholder="Например, УрФУ или Ельцин"
              autocomplete="off"
            >
            
            <transition name="fade">
              <ul v-if="isDropdownOpen && universitiesList?.length" class="autocomplete-dropdown">
                <li 
                  v-for="uni in universitiesList" 
                  :key="uni.name"
                  @click="selectUniversity(uni.name)"
                >
                  <span class="uni-name">{{ uni.name }}</span>
                </li>
              </ul>
            </transition>
          </div>
        </div>

        <div class="form-group">
          <label for="department">Институт / Кафедра:</label>
          <input 
            id="department" 
            type="text"
            list="departments-list"
            v-model="formData.department" 
            placeholder="ИРИТ-РТФ"
          >
          <datalist id="departments-list">
            <option v-for="item in suggestions.departments" :key="item" :value="item" />
          </datalist>
        </div>

        <div class="form-group">
          <label for="academicDegree">Ученая степень / Статус:</label>
          <input 
            id="academicDegree" 
            type="text"
            list="degrees-list"
            v-model="formData.academic_degree" 
            placeholder="Студент"
          >
          <datalist id="degrees-list">
            <option v-for="item in suggestions.degrees" :key="item" :value="item" />
          </datalist>
        </div>

        <div class="form-group">
          <label>Введите вашу дату рождения: <span class="required">*</span></label>
          <el-date-picker
            v-model="formData.birth_date"
            type="date"
            placeholder="ДД.ММ.ГГГГ"
            format="DD.MM.YYYY"
            value-format="YYYY-MM-DD"
            :disabled-date="disabledFutureDates"
            :editable="false"
            class="custom-date-picker"
          />
          <span v-if="errors.birth_date" class="error-text">{{ errors.birth_date }}</span>
        </div>

        <div class="form-group">
          <label for="phone">Введите ваш номер телефона:</label>
          <input 
            id="phone"
            type="tel"
            v-model="formData.phone" 
            @input="handlePhoneInput"
            placeholder="+7 (999) 999-99-99" 
            inputmode="tel"
            maxlength="18"
          >
        </div>

        <div class="checkbox-wrapper">
          <div class="checkbox-container">
            <input id="consent" type="checkbox" v-model="uiFields.consent">
            <label for="consent">Даю согласие на обработку персональных данных <span class="required">*</span></label>
          </div>
          <span v-if="errors.consent" class="error-text">{{ errors.consent }}</span>
        </div>
        
        <button 
          type="submit" 
          class="submit-button"
          :class="{ 'btn-disabled': hasAnyError }"
          :disabled="isLoading"
        >
          {{ isLoading ? 'Сохранение...' : 'Далее' }}
        </button>
      </form>

      <footer class="form-footer">
        <p>По всем вопросам можете обращаться:</p>
        <a href="mailto:adminexample@gmail.com">adminexample@gmail.com</a>
      </footer>
    </div>
  </div>
</template>

<script setup>
import { ref, reactive, computed, onMounted, onUnmounted } from 'vue';
import { useAuthStore } from '@/stores/auth';
import { useRouter } from 'vue-router';
import { getAllUniversities } from '@/services/api';

const authStore = useAuthStore();
const router = useRouter();
const isLoading = ref(false);

const formData = reactive({
  first_name: '',
  last_name: '',
  middle_name: '',
  university: '',
  department: '',
  academic_degree: '',
  role: 'participant', 
  phone: '',
  birth_date: '',
  gender: 'female',
});

const uiFields = reactive({
  consent: false,
});

const universitySearch = ref('');
const universitiesList = ref([]);
const isDropdownOpen = ref(false);
const universityContainer = ref(null);
let debounceTimeout = null;

const onUniversityInput = () => {
  isDropdownOpen.value = true;
  formData.university = universitySearch.value;
  clearTimeout(debounceTimeout);
  debounceTimeout = setTimeout(async () => {
    const query = universitySearch.value.trim();
    if (query.length < 2) {
      universitiesList.value = [];
      return;
    }
    try {
      const data = await getAllUniversities(query);
      console.log(data);
      universitiesList.value = data;
    } catch (err) {
      console.error(err);
    }
  }, 300);
};

const selectUniversity = (name) => {
  universitySearch.value = name;
  formData.university = name;
  isDropdownOpen.value = false;
};

const handleClickOutside = (event) => {
  if (universityContainer.value && !universityContainer.value.contains(event.target)) {
    isDropdownOpen.value = false;
  }
};

onMounted(() => {
  document.addEventListener('click', handleClickOutside);
});

onUnmounted(() => {
  document.removeEventListener('click', handleClickOutside);
});

const suggestions = {
  departments: [
    'ИРИТ-РТФ',
    'ИНЭУ',
    'ИЕНиМ',
    'Физико-технологический институт'
  ],
  degrees: [
    'Студент',
    'Магистрант',
    'Аспирант',
    'Кандидат технических наук',
    'Доктор наук',
    'Без ученой степени'
  ]
};

const errors = ref({});

const hasAnyError = computed(() => Object.keys(errors.value).length > 0);

const disabledFutureDates = (time) => {
  return time.getTime() > Date.now();
};

const handlePhoneInput = () => {
  let digits = formData.phone.replace(/\D/g, '');
  if (digits.startsWith('8')) digits = '7' + digits.slice(1);
  if (digits.startsWith('9')) digits = '7' + digits;
  digits = digits.slice(0, 11);
  if (!digits) { formData.phone = ''; return; }
  let out = '+7';
  if (digits.length > 1) out += ' (' + digits.slice(1, 4);
  if (digits.length >= 4) out += ') ' + digits.slice(4, 7);
  if (digits.length >= 7) out += '-' + digits.slice(7, 9);
  if (digits.length >= 9) out += '-' + digits.slice(9, 11);
  formData.phone = out;
};

const handleSubmit = async () => {
  errors.value = {};

  if (!formData.first_name.trim()) errors.value.first_name = 'Это поле обязательно';
  if (!formData.last_name.trim()) errors.value.last_name = 'Это поле обязательно';
  if (!formData.birth_date) errors.value.birth_date = 'Это поле обязательно';
  if (!uiFields.consent) errors.value.consent = 'Это поле обязательно';

  if (Object.keys(errors.value).length === 0) {
    try {
      isLoading.value = true;
      const data = await authStore.completeRegistration({
        ...formData
      });
      if (data.user.role === 'organizer') {
        router.push({ name: 'OrganizerDashboard' });
      } else if (data.user.role === 'moderator') {
        router.push({ name: 'ModeratorDashboard' });
      } else {
        router.push({ name: 'Dashboard' });
      }
    } catch (err) {
      console.error(err);
    } finally {
      isLoading.value = false;
    }
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
  box-sizing: border-box;
}

.form-container {
  background-color: variables.$white;
  border-radius: functions.radius('l');
  padding: 40px 32px;
  width: 100%;
  max-width: 440px;
  text-align: left;
  box-shadow: 0 4px 20px rgba(0, 0, 0, 0.02);
  box-sizing: border-box;
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
    font-size: 14px;
    font-weight: 500;
    color: variables.$black;
    margin-bottom: 8px;
    
  }
}

.required { color: variables.$dangerous; }

input[type="text"],
input[type="tel"] {
  width: 100%;
  padding: 14px 20px;
  background-color: variables.$dark-white;
  border: 1px solid transparent;
  border-radius: functions.radius('m');
  font-size: 16px;
  color: variables.$black;
  transition: all 0.2s;
  box-sizing: border-box;

  &::placeholder { color: variables.$gray; opacity: 0.6; }
  &:focus { outline: none; background-color: variables.$white; border-color: variables.$main-color; }
  &.input-error { border-color: variables.$dangerous; background-color: variables.$white; }
}

/* Стилизация анимированных слайдеров (Роль и Пол) */
.slider-selector {
  position: relative;
  display: flex;
  background-color: variables.$dark-white;
  border-radius: functions.radius('m');
  padding: 4px;
  box-sizing: border-box;
  height: 50px;

  .slider-active-bg {
    position: absolute;
    top: 4px;
    bottom: 4px;
    left: 4px;
    width: calc(50% - 4px);
    background-color: variables.$main-color;
    border-radius: functions.radius('s');
    transition: transform 0.25s cubic-bezier(0.4, 0, 0.2, 1);
    transform: translateX(calc(100% * var(--active-index)));
    z-index: 1;
  }

  .slider-btn {
    position: relative;
    flex: 1;
    border: none;
    background: transparent;
    color: variables.$gray;
    font-size: 16px;
    font-weight: 500;
    cursor: pointer;
    transition: color 0.2s ease;
    z-index: 2;

    &.active {
      color: variables.$white;
    }
  }
}

/* Полная кастомизация Element Plus под ваши токены */
:deep(.el-date-editor.custom-date-picker) {
  width: 100% !important;
  height: 50px;
  
  .el-input__wrapper {
    background-color: variables.$dark-white !important;
    border-radius: functions.radius('m') !important;
    padding: 14px 20px !important;
    box-shadow: none !important;
    border: 1px solid transparent;
    box-sizing: border-box;
    flex-direction: row-reverse; /* Переносим иконку календаря вправо как на дизайне */

    &:hover, &.is-focus {
      border-color: variables.$main-color !important;
      background-color: variables.$white !important;
    }

    .el-input__inner {
      color: variables.$black !important;
      font-size: 16px !important;
      font-family: inherit;
      text-align: left;
      &::placeholder {
        color: variables.$gray !important;
        opacity: 0.6;
      }
    }

    .el-input__prefix {
      margin-left: auto;
      color: variables.$gray;
    }
  }
}

.error-text {
  color: variables.$dangerous;
  font-size: 12px;
  margin-top: 4px;
  display: block;
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
      accent-color: variables.$main-color;
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
  transition: opacity 0.2s;

  &:hover { opacity: 0.9; }
  &.btn-disabled { background-color: variables.$ultra-light-main-color; color: variables.$light-main-color; cursor: not-allowed; }
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