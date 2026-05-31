<template>
    <div class="auth-container">
        <div class="auth-card">
            <header class="header">
                <button class="back-btn" @click="router.back()">
                    <svg width="10" height="16" viewBox="0 0 10 16" fill="none">
                        <path d="M8.5 15L1.5 8L8.5 1" stroke="#8C93A1" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"/>
                    </svg>
                </button>
                <h1 class="title">{{ $t('auth.mailConfirmation.title') }}</h1>
            </header>

            <div class="message-block">
                <p v-if="!hasError" class="info-text">{{ $t('auth.mailConfirmation.infoText') }}</p>
                <p v-else class="error-message">{{ $t('auth.mailConfirmation.errorText') }}</p>
            </div>

            <div class="otp-inputs">
                <input
                    v-for="(digit, index) in 6"
                    :key="index"
                    type="text"
                    inputmode="numeric"
                    maxlength="1"
                    v-model="code[index]"
                    class="otp-field"
                    :class="{
                        'field-error': hasError,
                        'field-active': code[index] !== '' && !hasError
                    }"
                    @input="handleInput($event, index)"
                    @keydown.delete="handleDelete($event, index)"
                    @paste="handlePaste($event)"
                    ref="inputs"
                />
            </div>

            <div class="timer-block">
                <p v-if="timer > 0" class="timer-text">
                    {{ $t('auth.mailConfirmation.timerText', { seconds: timer }) }}
                </p>
                <button v-else type="button" class="resend-btn" @click="resendCode">
                    {{ $t('auth.mailConfirmation.resendBtn') }}
                </button>
            </div>

            <button
                class="submit-btn"
                :class="{ 'btn-error-state': hasError }"
                :disabled="!isCodeComplete"
                @click="verifyCode"
            >
                <span v-if="isVerifying" class="btn-loading"></span>
                <span v-else>{{ $t('auth.mailConfirmation.submitBtn') }}</span>
            </button>

            <footer class="footer">
                <p>{{ $t('common.contactText') }}</p>
                <a :href="'mailto:' + $t('common.contactEmail')">{{ $t('common.contactEmail') }}</a>
            </footer>

            <LangSwitch />
        </div>
    </div>
</template>

<script setup>
import { ref, computed, onMounted } from 'vue'
import { useRouter } from 'vue-router'
import { useAuthStore } from '@/stores/auth'
import { useI18n } from 'vue-i18n'
import { ElMessage } from 'element-plus'
import LangSwitch from '@/components/LangSwitch.vue'

const props = defineProps({ mode: { type: String, default: 'registration' } })
const { t } = useI18n()
const router = useRouter()
const code = ref(['', '', '', '', '', ''])
const inputs = ref([])
const timer = ref(119)
const hasError = ref(false)
const isVerifying = ref(false)

const isCodeComplete = computed(() => code.value.every(d => d !== ''))

onMounted(() => {
    startTimer()
    // Focus first input
    setTimeout(() => inputs.value[0]?.focus(), 100)
})

function startTimer() {
    const countdown = setInterval(() => {
        if (timer.value > 0) timer.value--
        else clearInterval(countdown)
    }, 1000)
}

const handleInput = (e, index) => {
    const val = e.target.value
    hasError.value = false
    if (!/^\d*$/.test(val)) { code.value[index] = ''; return }
    if (val && index < 5) inputs.value[index + 1]?.focus()
}

const handleDelete = (e, index) => {
    if (!code.value[index] && index > 0) inputs.value[index - 1]?.focus()
}

const handlePaste = (e) => {
    const text = e.clipboardData.getData('text').replace(/\D/g, '').slice(0, 6)
    if (text.length === 6) {
        text.split('').forEach((c, i) => { code.value[i] = c })
        inputs.value[5]?.focus()
        e.preventDefault()
    }
}

const verifyCode = async () => {
    if (!isCodeComplete.value) return
    const enteredCode = code.value.join('')
    const authStore = useAuthStore()
    isVerifying.value = true
    try {
        await authStore.verifyEmail(enteredCode)
        router.push(props.mode === 'registration' ? '/final-registration' : '/new-password')
    } catch (error) {
        hasError.value = true
        ElMessage.error(error.response?.data?.error || t('auth.mailConfirmation.errorText'))
    } finally {
        isVerifying.value = false
    }
}

const resendCode = () => {
    timer.value = 119
    hasError.value = false
    code.value = ['', '', '', '', '', '']
    startTimer()
    inputs.value[0]?.focus()
}
</script>

<style scoped lang="scss">
.auth-container {
    display: flex;
    justify-content: center;
    align-items: center;
    min-height: 100vh;
    background-color: variables.$dark-white;
}

.auth-card {
    width: 100%;
    max-width: 420px;
    padding: 32px 24px;
    text-align: center;
    background-color: variables.$white;
    border-radius: functions.radius('xxl');
}

.header {
    position: relative;
    margin-bottom: 24px;
}

.back-btn {
    position: absolute;
    left: 0;
    top: 10px;
    background: none;
    border: none;
    cursor: pointer;
    padding: 5px;
}

.title {
    font-size: 32px;
    font-weight: 700;
    color: variables.$black;
    line-height: 1.2;
    margin: 0;
    white-space: pre-line;
}

.message-block { margin-bottom: 28px; }

.info-text {
    color: variables.$gray;
    font-size: 15px;
    line-height: 1.5;
    margin: 0;
    white-space: pre-line;
}

.error-message {
    color: variables.$dangerous;
    font-size: 14px;
    line-height: 1.5;
    margin: 0;
    white-space: pre-line;
}

.otp-inputs {
    display: flex;
    justify-content: center;
    gap: 10px;
    margin-bottom: 24px;
}

.otp-field {
    width: 45px;
    height: 55px;
    border: 1px solid variables.$light-main-color;
    border-radius: functions.radius('s');
    text-align: center;
    font-size: 24px;
    font-weight: 600;
    color: variables.$light-main-color;
    background-color: variables.$white;
    outline: none;
    transition: all 0.2s;
    font-family: inherit;
}

.otp-field:focus {
    border-color: variables.$main-color;
    background-color: variables.$dark-white;
    color: variables.$main-color;
}

.field-error {
    border-color: variables.$dangerous;
    color: variables.$dangerous;
}

.field-active {
    border-color: variables.$main-color;
    color: variables.$main-color;
}

.timer-block {
    background-color: variables.$dark-white;
    border-radius: functions.radius('s');
    padding: 14px;
    margin-bottom: 24px;
}

.timer-text {
    color: variables.$gray;
    font-size: 14px;
    margin: 0;
}

.resend-btn {
    background: none;
    border: none;
    color: variables.$light-main-color;
    font-size: 14px;
    cursor: pointer;
    font-weight: 600;
    font-family: inherit;
    transition: color 0.2s;

    &:hover { color: variables.$main-color; }
}

.submit-btn {
    width: 100%;
    padding: 16px;
    background-color: variables.$main-color;
    color: variables.$white;
    border: none;
    border-radius: functions.radius('s');
    font-size: 17px;
    font-weight: 600;
    cursor: pointer;
    transition: opacity 0.2s;
    font-family: inherit;
    display: flex;
    align-items: center;
    justify-content: center;
    gap: 8px;
}

.submit-btn:disabled { opacity: 0.7; cursor: not-allowed; }
.btn-error-state { background-color: variables.$dangerous; }

.btn-loading {
    width: 18px;
    height: 18px;
    border: 2px solid rgba(255, 255, 255, 0.4);
    border-top-color: variables.$white;
    border-radius: 50%;
    animation: spin 0.8s linear infinite;
}

@keyframes spin { to { transform: rotate(360deg); } }

.footer {
    margin-top: 24px;
    font-size: 13px;
    color: variables.$gray;
    line-height: 1.6;
    a { color: variables.$gray; text-decoration: none; }
    a:hover { text-decoration: underline; }
}
</style>