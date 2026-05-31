<template>
    <div class="form-wrapper">
        <div class="form-container">
            <header class="form-header">
                <h1>{{ $t('auth.passwordRecovery.title') }}</h1>
                <p>{{ $t('auth.passwordRecovery.description') }}</p>
            </header>

            <form @submit.prevent="handleSubmit" novalidate>
                <div class="form-group">
                    <input
                        type="email"
                        v-model="email"
                        :placeholder="$t('auth.passwordRecovery.emailPlaceholder')"
                        :class="{ 'has-error': error }"
                        :aria-label="$t('auth.passwordRecovery.emailPlaceholder')"
                        @input="error = ''"
                    >
                    <span v-if="error" class="error-message">{{ error }}</span>
                </div>

                <div class="button-group">
                    <button type="submit" class="submit-button" :disabled="isLoading">
                        <span v-if="isLoading" class="btn-loading"></span>
                        <span v-else>{{ $t('auth.passwordRecovery.submitBtn') }}</span>
                    </button>
                    <button type="button" class="cancel-button" @click="handleCancel">
                        {{ $t('auth.passwordRecovery.cancelBtn') }}
                    </button>
                </div>
            </form>

            <footer class="form-footer">
                <p>{{ $t('common.contactText') }}<br>{{ $t('common.contactEmail') }}</p>
            </footer>

            <LangSwitch />
        </div>
    </div>
</template>

<script setup>
import { ref } from 'vue'
import { useRouter } from 'vue-router'
import { useI18n } from 'vue-i18n'
import LangSwitch from '@/components/LangSwitch.vue'

const { t } = useI18n()
const router = useRouter()
const email = ref('')
const error = ref('')
const isLoading = ref(false)

const validateEmail = (val) => {
    if (!val) return t('auth.passwordRecovery.errors.emailRequired')
    const re = /^\S+@\S+\.\S+$/
    if (!re.test(val)) return t('auth.passwordRecovery.errors.invalidEmail')
    return ''
}

const handleSubmit = async () => {
    error.value = validateEmail(email.value)
    if (!error.value) {
        isLoading.value = true
        try {
            // Call API to send reset code
            // await authStore.requestPasswordReset(email.value)
            router.push('/reset-password')
        } finally {
            isLoading.value = false
        }
    }
}

const handleCancel = () => {
    email.value = ''
    error.value = ''
    router.back()
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
    max-width: 420px;
    text-align: center;
}

.form-header h1 {
    font-size: 32px;
    font-weight: 700;
    margin: 0 0 16px 0;
    line-height: 1.2;
}

.form-header p {
    font-size: 15px;
    color: variables.$gray;
    line-height: 1.6;
    margin: 0 0 32px 0;
}

.form-group { margin-bottom: 24px; }

input[type="email"] {
    width: 100%;
    padding: 16px;
    background-color: variables.$dark-white;
    border: 1px solid transparent;
    border-radius: functions.radius('m');
    font-size: 16px;
    box-sizing: border-box;
    text-align: left;
    transition: border-color 0.2s, box-shadow 0.2s;
    font-family: inherit;
    color: variables.$black;
    outline: none;
}

input[type="email"].has-error { border-color: variables.$dangerous; }

.error-message {
    color: variables.$dangerous;
    font-size: 13px;
    margin-top: 8px;
    display: block;
    text-align: left;
}

.button-group {
    display: flex;
    flex-direction: column;
    gap: 12px;
}

.submit-button,
.cancel-button {
    width: 100%;
    padding: 16px;
    border-radius: functions.radius('m');
    font-size: 17px;
    font-weight: 600;
    cursor: pointer;
    border: none;
    transition: all 0.2s;
    font-family: inherit;
    display: flex;
    align-items: center;
    justify-content: center;
    gap: 8px;
}

.submit-button {
    background-color: variables.$main-color;
    color: variables.$white;
    &:disabled { opacity: 0.7; cursor: not-allowed; }
}

.cancel-button {
    background-color: variables.$dark-white;
    color: variables.$black;
    font-weight: 500;
}

.btn-loading {
    width: 18px;
    height: 18px;
    border: 2px solid rgba(255, 255, 255, 0.4);
    border-top-color: variables.$white;
    border-radius: 50%;
    animation: spin 0.8s linear infinite;
}

@keyframes spin { to { transform: rotate(360deg); } }

.form-footer {
    margin-top: 32px;
    font-size: 13px;
    color: variables.$gray;
    line-height: 1.5;
    p { margin: 0; }
}
</style>