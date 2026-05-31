<template>
    <div class="lang-switch">
        <span :class="{ 'active-lang': currentLang === 'ru' }" @click="switchLang('ru')">RU</span>
        <span :class="{ 'active-lang': currentLang === 'en' }" @click="switchLang('en')">EN</span>
    </div>
</template>

<script setup>
import { computed } from 'vue'
import { useI18n } from 'vue-i18n'

const { locale } = useI18n()
const currentLang = computed(() => locale.value)

function switchLang(lang) {
    locale.value = lang
    localStorage.setItem('app_language', lang)
}
</script>

<style scoped lang="scss">
.lang-switch {
    display: flex;
    align-items: center;
    justify-content: center;
    gap: 8px;
    margin-top: 24px;
}

.lang-switch span {
    padding: 6px 12px;
    font-size: 13px;
    color: variables.$gray;
    border-radius: functions.radius('xs');
    cursor: pointer;
    transition: all 0.2s;
    user-select: none;
}

.lang-switch span.active-lang {
    background: variables.$dark-white;
    color: variables.$black;
    font-weight: 600;
}

.lang-switch span:hover:not(.active-lang) {
    color: variables.$black;
}
</style>