import { createI18n } from 'vue-i18n'
import { createApp } from 'vue'
import en from './auth/login/en.js'
import ru from './auth/login/ru.js'

const i18n = createI18n({
    legacy: false,
    locale: 'en',
    fallbackLocale: 'en',
    messages: {
        en,
        ru,
    },
});

export default i18n;