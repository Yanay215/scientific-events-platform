import { createI18n } from 'vue-i18n'
import en from './en.js'
import ru from './ru.js'

const savedLang = localStorage.getItem('app_language') || 'ru'

const i18n = createI18n({
    legacy: false,
    locale: savedLang,
    fallbackLocale: 'ru',
    messages: {
        en,
        ru,
    },
});

export default i18n;