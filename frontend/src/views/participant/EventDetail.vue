<template>
    <div class="form-wrapper">
        <div class="form-container">
            <header class="form-header">
                <button class="back-btn" @click="$router.back()">
                    <svg width="10" height="16" viewBox="0 0 10 16" fill="none">
                        <path d="M8.5 15L1.5 8L8.5 1" stroke="#8C93A1" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"/>
                    </svg>
                </button>
                <h1>{{ $t('auth.newPassword.title') }}</h1>
            </header>

            <div v-if="globalError" class="global-error-message">{{ globalError }}</div>

            <form @submit.prevent="handleSubmit" class="main-form">
                <div class="strength-indicator">
                    <div class="strength-label">{{ $t('auth.newPassword.strength.label') }}</div>
                    <div class="strength-visual">
                        <div class="strength-bar-bg">
                            <div class="strength-bar-fill" :style="passwordStrength.style"></div>
                        </div>
                        <span class="strength-text" v-if="passwordStrength.text">{{ passwordStrength.text }}</span>
                    </div>
                </div>

                <div class="form-group">
                    <div class="password-input-wrapper">
                        <input
                            :type="passwordFieldType"
                            v-model="password"
                            :placeholder="$t('auth.newPassword.passwordPlaceholder')"
                            :class="{ 'is-invalid': passwordError || globalError }"
                            @input="globalError = ''"
                        >
                        <button type="button" @click="togglePasswordVisibility('password')" class="toggle-visibility">
                            <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                                <path v-if="passwordFieldType === 'password'" d="M1 12s4-8 11-8 11 8 11 8-4 8-11 8-11-8-11-8z"/>
                                <circle v-if="passwordFieldType === 'password'" cx="12" cy="12" r="3"/>
                                <path v-else d="M17.94 17.94A10.07 10.07 0 0 1 12 20c-7 0-11-8-11-8a18.45 18.45 0 0 1 5.06-5.94M9.9 4.24A9.12 9.12 0 0 1 12 4c7 0 11 8 11 8a18.5 18.5 0 0 1-2.16 3.19m-6.72-1.07a3 3 0 1 1-4.24-4.24"/>
                                <line v-if="passwordFieldType === 'text'" x1="1" y1="1" x2="23" y2="23"/>
                            </svg>
                        </button>
                    </div>
                </div>

                <div class="form-group">
                    <div class="password-input-wrapper">
                        <input
                            :type="confirmPasswordFieldType"
                            v-model="passwordConfirm"
                            :placeholder="$t('auth.newPassword.repeatPasswordPlaceholder')"
                            :class="{ 'is-invalid': passwordConfirmError || globalError }"
                            @input="globalError = ''"
                        >
                        <button type="button" @click="togglePasswordVisibility('confirm')" class="toggle-visibility">
                            <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                                <path v-if="confirmPasswordFieldType === 'password'" d="M1 12s4-8 11-8 11 8 11 8-4 8-11 8-11-8-11-8z"/>
                                <circle v-if="confirmPasswordFieldType === 'password'" cx="12" cy="12" r="3"/>
                                <path v-else d="M17.94 17.94A10.07 10.07 0 0 1 12 20c-7 0-11-8-11-8a18.45 18.45 0 0 1 5.06-5.94M9.9 4.24A9.12 9.12 0 0 1 12 4c7 0 11 8 11 8a18.5 18.5 0 0 1-2.16 3.19m-6.72-1.07a3 3 0 1 1-4.24-4.24"/>
                                <line v-if="confirmPasswordFieldType === 'text'" x1="1" y1="1" x2="23" y2="23"/>
                            </svg>
                        </button>
                    </div>
                </div>

                <p class="password-hint">{{ $t('auth.newPassword.hint') }}</p>

                <button
                    type="submit"
                    class="submit-button"
                    :class="{ 'btn-error': globalError || (!isPasswordValid && submitted) }"
                >
                    {{ $t('auth.newPassword.submitBtn') }}
                </button>
            </form>

            <footer class="form-footer">
                <p>{{ $t('common.contactText') }}</p>
                <a :href="'mailto:' + $t('common.contactEmail')">{{ $t('common.contactEmail') }}</a>
            </footer>

            <LangSwitch />
        </div>
    </div>
</template>

<script setup>
import { ref, computed } from 'vue'
import { useRouter } from 'vue-router'
import { useI18n } from 'vue-i18n'
import LangSwitch from '@/components/LangSwitch.vue'

const { t } = useI18n()
const router = useRouter()
const password = ref('')
const passwordConfirm = ref('')
const passwordFieldType = ref('password')
const confirmPasswordFieldType = ref('password')
const globalError = ref('')
const submitted = ref(false)

const passwordError = computed(() => submitted.value && !isPasswordValid.value)
const passwordConfirmError = computed(() => submitted.value && password.value !== passwordConfirm.value)

const togglePasswordVisibility = (field) => {
    if (field === 'password') {
        passwordFieldType.value = passwordFieldType.value === 'password' ? 'text' : 'password'
    } else {
        confirmPasswordFieldType.value = confirmPasswordFieldType.value === 'password' ? 'text' : 'password'
    }
}

const passwordStrength = computed(() => {
    if (!password.value) return { text: '', style: { width: '0%' } }
    let score = 0
    if (password.value.length >= 8) score++
    if (/[A-Z]/.test(password.value)) score++
    if (/[0-9]/.test(password.value)) score++
    if (score <= 1) return { text: t('auth.newPassword.strength.weak'), style: { width: '33%', backgroundColor: '#e63946' } }
    if (score === 2) return { text: t('auth.newPassword.strength.normal'), style: { width: '66%', backgroundColor: '#FFB300' } }
    return { text: t('auth.newPassword.strength.strong'), style: { width: '100%', backgroundColor: '#1f7a5a' } }
})

const isPasswordValid = computed(() =>
    password.value.length >= 8 && /[A-Z]/.test(password.value) && /[0-9]/.test(password.value)
)

const handleSubmit = () => {
    submitted.value = true
    globalError.value = ''
    if (!isPasswordValid.value) return
    if (password.value !== passwordConfirm.value) {
        globalError.value = t('auth.newPassword.errors.mismatch')
        return
    }
    // TODO: call API to reset password
    router.push('/login')
}
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
    text-align: center;
}

.form-header {
    position: relative;
    margin-bottom: 32px;
}

.back-btn {
    position: absolute;
    left: 0;
    top: 6px;
    background: none;
    border: none;
    cursor: pointer;
    padding: 5px;
}

.form-header h1 {
    font-size: 32px;
    font-weight: 700;
    color: variables.$black;
    line-height: 1.2;
    margin: 0;
    white-space: pre-line;
}

.global-error-message {
    color: variables.$dangerous;
    font-size: 14px;
    margin-bottom: 20px;
    font-weight: 500;
}

.strength-indicator {
    text-align: left;
    margin-bottom: 24px;

    .strength-label {
        font-size: 14px;
        color: variables.$gray;
        margin-bottom: 8px;
    }

    .strength-visual { display: flex; flex-direction: column; gap: 8px; }

    .strength-bar-bg {
        height: 6px;
        background-color: variables.$dark-white;
        border-radius: 10px;
        overflow: hidden;
    }

    .strength-bar-fill {
        height: 100%;
        transition: width 0.3s ease, background-color 0.3s ease;
    }

    .strength-text {
        font-size: 13px;
        color: variables.$gray;
        text-align: right;
    }
}

.form-group { margin-bottom: 16px; }

.password-input-wrapper {
    position: relative;
    input {
        width: 100%;
        padding: 16px 50px 16px 20px;
        background-color: variables.$dark-white;
        border: 1px solid transparent;
        border-radius: functions.radius('m');
        font-size: 16px;
        color: variables.$black;
        box-sizing: border-box;
        transition: all 0.2s;
        outline: none;
        font-family: inherit;

        &::placeholder { color: variables.$gray; }
        &:focus { background-color: variables.$white; border-color: variables.$light-main-color; }
        &.is-invalid { border-color: variables.$dangerous; background-color: variables.$white; }
    }
}

.toggle-visibility {
    position: absolute;
    right: 16px;
    top: 50%;
    transform: translateY(-50%);
    background: none;
    border: none;
    cursor: pointer;
    color: variables.$gray;
    display: flex;
    align-items: center;
    padding: 0;
    &:hover { color: variables.$black; }
}

.password-hint {
    font-size: 12px;
    color: variables.$gray;
    line-height: 1.4;
    margin: 16px 0 28px;
    text-align: center;
}

.submit-button {
    width: 100%;
    padding: 18px;
    background-color: variables.$main-color;
    color: variables.$white;
    border: none;
    border-radius: functions.radius('m');
    font-size: 17px;
    font-weight: 600;
    cursor: pointer;
    transition: background-color 0.2s;
    font-family: inherit;

    &.btn-error { background-color: variables.$dangerous; }
}

.form-footer {
    margin-top: 28px;
    font-size: 13px;
    color: variables.$gray;
    p { margin: 0 0 4px; }
    a { color: variables.$gray; text-decoration: none; }
    a:hover { text-decoration: underline; }
}
</style>