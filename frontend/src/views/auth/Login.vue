<template>
    <div class="login-container">
        <div v-if="accountNotFound" class="login-card">
            <h1 class="login-title">{{ $t('auth.notFound.title') }}</h1>
            <p class="not-found-hint" v-html="$t('auth.notFound.hint')"></p>
            <button class="submit-btn" @click="resetForm">{{ $t('auth.notFound.tryAgain') }}</button>
            <p class="contact-text">
                {{ $t('common.contactText') }}<br>
                <a :href="'mailto:' + $t('common.contactEmail')">{{ $t('common.contactEmail') }}</a>
            </p>
        </div>

        <div v-else class="login-card">
            <h1 class="login-title">{{ $t('auth.login.title') }}</h1>

            <transition name="fade-slide">
                <p v-if="errorMessage" class="error-text">{{ errorMessage }}</p>
            </transition>

            <form @submit.prevent="handleLogin" class="form">
                <div class="field-wrap">
                    <div :class="['field', errors.email ? 'field-error-wrap' : '']">
                        <input
                            v-model="email"
                            type="email"
                            class="input"
                            :placeholder="$t('auth.login.emailPlaceholder')"
                            autocomplete="email"
                            @blur="validateEmail"
                            @input="errors.email = false"
                        />
                    </div>
                    <span v-if="errors.email" class="field-error-text">{{ $t('auth.login.errors.invalidEmail') }}</span>
                </div>

                <div class="field-wrap">
                    <div :class="['field', errors.password ? 'field-error-wrap' : '']">
                        <input
                            v-model="password"
                            :type="showPassword ? 'text' : 'password'"
                            class="input input--password"
                            :placeholder="$t('auth.login.passwordPlaceholder')"
                            autocomplete="current-password"
                        />
                        <button type="button" class="eye-btn" @click="showPassword = !showPassword">
                            <svg v-if="!showPassword" width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="#8C93A1" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                                <path d="M1 12s4-8 11-8 11 8 11 8-4 8-11 8-11-8-11-8z"></path>
                                <circle cx="12" cy="12" r="3"></circle>
                            </svg>
                            <svg v-else width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="#8C93A1" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                                <path d="M17.94 17.94A10.07 10.07 0 0 1 12 20c-7 0-11-8-11-8a18.45 18.45 0 0 1 5.06-5.94M9.9 4.24A9.12 9.12 0 0 1 12 4c7 0 11 8 11 8a18.5 18.5 0 0 1-2.16 3.19m-6.72-1.07a3 3 0 1 1-4.24-4.24"></path>
                                <line x1="1" y1="1" x2="23" y2="23"></line>
                            </svg>
                        </button>
                    </div>
                    <span v-if="errors.password" class="field-error-text">{{ $t('auth.login.errors.invalidPassword') }}</span>
                </div>

                <div class="remember-forgot">
                    <label class="remember">
                        <input type="checkbox" v-model="rememberMe" class="checkbox" />
                        <span>{{ $t('auth.login.rememberMe') }}</span>
                    </label>
                    <RouterLink to="/forgot-password" class="forgot-link">
                        {{ $t('auth.login.forgotPassword') }}
                    </RouterLink>
                </div>

                <button
                    type="submit"
                    class="submit-btn"
                    :disabled="isLoading || !isFormValid"
                    :class="{ 'inactive-btn': !isFormValid && !isLoading }"
                >
                    <span v-if="isLoading" class="submit-btn--loading"></span>
                    <span v-else>{{ $t('auth.login.submit') }}</span>
                </button>
            </form>

            <button class="create-account-btn" @click="$router.push('/register')">
                {{ $t('auth.login.createAccount') }}
            </button>

            <p class="contact-text">
                {{ $t('common.contactText') }}<br>
                <a :href="'mailto:' + $t('common.contactEmail')">{{ $t('common.contactEmail') }}</a>
            </p>

            <div class="lang-switch">
                <span :class="{ 'active-lang': currentLang === 'ru' }" @click="switchLanguage('ru')">RU</span>
                <span :class="{ 'active-lang': currentLang === 'en' }" @click="switchLanguage('en')">EN</span>
            </div>
        </div>
    </div>
</template>

<script setup>
import { ref, computed } from 'vue'
import { useI18n } from 'vue-i18n'
import { useRouter } from 'vue-router'
import { useAuthStore } from '../../stores/auth'

const { t, locale } = useI18n()
const router = useRouter()
const authStore = useAuthStore()

const email = ref('')
const password = ref('')
const rememberMe = ref(false)
const showPassword = ref(false)
const isLoading = ref(false)
const errorMessage = ref('')
const accountNotFound = ref(false)
const errors = ref({ email: false, password: false })

const currentLang = computed(() => locale.value)

const isFormValid = computed(() =>
    email.value.trim() !== '' && password.value.trim() !== '' && !errors.value.email
)

function validateEmail() {
    const re = /^[^\s@]+@[^\s@]+\.[^\s@]+$/
    errors.value.email = email.value ? !re.test(email.value) : false
}

function switchLanguage(lang) {
    locale.value = lang
    localStorage.setItem('app_language', lang)
}

async function handleLogin(e) {
    if (e && e.preventDefault) e.preventDefault()
    if (!isFormValid.value) {
        validateEmail()
        errors.value.password = password.value.trim() === ''
        return
    }
    errors.value = { email: false, password: false }
    isLoading.value = true
    errorMessage.value = ''
    try {
        await authStore.login(email.value, password.value)
        const role = authStore.userRole
        if (role === 'participant') router.push('/participant/dashboard')
        else if (role === 'organizer') router.push('/organizer/events')
        else if (role === 'moderator') router.push('/moderator/queue')
    } catch (err) {
        if (err?.response?.status === 404) {
            accountNotFound.value = true
        } else {
            errorMessage.value = err?.response?.data?.error || t('auth.login.errors.loginFailed')
        }
    } finally {
        isLoading.value = false
    }
}

function resetForm() {
    accountNotFound.value = false
    errorMessage.value = ''
    email.value = ''
    password.value = ''
    errors.value = { email: false, password: false }
}
</script>

<style scoped lang="scss">
.login-container {
    display: flex;
    justify-content: center;
    align-items: center;
    min-height: 100vh;
    background-color: variables.$dark-white;
}

.login-card {
    width: 100%;
    max-width: 420px;
    padding: 40px;
    background-color: variables.$white;
    box-sizing: border-box;
    border-radius: functions.radius('xl');
}

.login-title {
    text-align: center;
    font-size: 32px;
    font-weight: 700;
    color: variables.$black;
    margin-top: 0;
    margin-bottom: 32px;
}

.not-found-hint {
    text-align: center;
    color: variables.$gray;
    font-size: 15px;
    margin-bottom: 24px;
}

.error-text {
    color: variables.$dangerous;
    font-size: 14px;
    margin-bottom: 16px;
    text-align: center;
}

.fade-slide-enter-active,
.fade-slide-leave-active { transition: opacity 0.3s ease; }
.fade-slide-enter-from,
.fade-slide-leave-to { opacity: 0; }

.form {
    display: flex;
    flex-direction: column;
    gap: 20px;
    margin-bottom: 10px;
}

.field-wrap {
    display: flex;
    flex-direction: column;
    gap: 4px;
}

.field {
    position: relative;
    display: flex;
    align-items: center;
}

input.input {
    width: 100%;
    padding: 16px 20px;
    border: 2px solid transparent;
    border-radius: functions.radius('m');
    font-size: 16px;
    color: variables.$black;
    background: variables.$dark-white;
    outline: none;
    box-sizing: border-box;
    transition: all 0.2s ease;
}

input.input::placeholder { color: variables.$gray; }
.field-error-wrap input.input { border-color: variables.$dangerous; }
input.input--password { padding-right: 48px; }

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
    transition: color 0.2s;
}

.field-error-text {
    color: variables.$dangerous;
    font-size: 12px;
    padding-left: 4px;
}

.remember-forgot {
    display: flex;
    align-items: center;
    justify-content: space-between;
    margin-top: -4px;
    margin-bottom: 8px;
}

.remember {
    display: flex;
    align-items: center;
    gap: 8px;
    font-size: 14px;
    color: variables.$gray;
    cursor: pointer;
    user-select: none;
}

.checkbox {
    width: 18px;
    height: 18px;
    cursor: pointer;
    border-radius: functions.radius('xxs');
    accent-color: variables.$main-color;
}

.forgot-link {
    font-size: 14px;
    color: variables.$light-main-color;
    text-decoration: none;
    transition: color 0.2s;
}

.submit-btn {
    width: 100%;
    padding: 16px;
    background: variables.$main-color;
    color: variables.$white;
    font-size: 16px;
    font-weight: 600;
    border: none;
    border-radius: functions.radius('m');
    cursor: pointer;
    transition: background 0.2s, opacity 0.2s;
    display: flex;
    align-items: center;
    justify-content: center;
    margin-bottom: 4px;
}

.submit-btn:disabled { opacity: 0.7; cursor: not-allowed; }
.inactive-btn { opacity: 0.8; }

.submit-btn--loading {
    width: 20px;
    height: 20px;
    border: 2px solid rgba(255, 255, 255, 0.4);
    border-top-color: variables.$white;
    border-radius: 50%;
    animation: spin 0.8s linear infinite;
}

@keyframes spin { to { transform: rotate(360deg); } }

.create-account-btn {
    width: 100%;
    padding: 16px;
    background: variables.$dark-white;
    color: variables.$gray;
    font-size: 16px;
    font-weight: 500;
    border: none;
    border-radius: functions.radius('m');
    cursor: pointer;
    transition: background 0.2s;
    margin-bottom: 16px;

    &:hover { background: darken(#f9f9f9, 3%); }
}

.contact-text {
    font-size: 13px;
    color: variables.$gray;
    text-align: center;
    line-height: 1.5;
    margin-top: 16px;
}

.contact-text a {
    color: variables.$gray;
    text-decoration: none;
    &:hover { text-decoration: underline; }
}

.lang-switch {
    display: flex;
    align-items: center;
    justify-content: center;
    gap: 8px;
    margin-top: 20px;
}

.lang-switch span {
    padding: 6px 12px;
    font-size: 13px;
    color: variables.$gray;
    border-radius: functions.radius('xs');
    cursor: pointer;
    transition: all 0.2s;
}

.lang-switch span.active-lang {
    background: variables.$dark-white;
    color: variables.$black;
    font-weight: 600;
}
</style>