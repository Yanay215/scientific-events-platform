<template>
    <header class="app-header">
        <div class="logo">{{ $t('common.appName') }}</div>
        <nav class="nav-links">
            <RouterLink to="/participant/dashboard" class="nav-link" v-if="role === 'participant'">
                {{ $t('header.events') }}
            </RouterLink>
            <RouterLink to="/organizer/events" class="nav-link" v-if="role === 'organizer'">
                {{ $t('header.events') }}
            </RouterLink>
            <RouterLink to="/moderator/queue" class="nav-link" v-if="role === 'moderator'">
                {{ $t('common.users') }}
            </RouterLink>
        </nav>
        <div class="header-actions">
            <button class="notification-btn" :title="$t('header.notifications')">
                <svg width="22" height="22" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                    <path d="M18 8A6 6 0 0 0 6 8c0 7-3 9-3 9h18s-3-2-3-9"></path>
                    <path d="M13.73 21a2 2 0 0 1-3.46 0"></path>
                </svg>
                <span class="badge" v-if="notificationCount > 0">{{ notificationCount }}</span>
            </button>

            <div class="profile-menu" ref="menuRef">
                <button class="profile-btn" @click="toggleMenu" :title="$t('header.profile')">
                    <svg width="22" height="22" viewBox="0 0 24 24" fill="none" stroke="#a8a8a8" stroke-width="2">
                        <circle cx="12" cy="7" r="4"></circle>
                        <path d="M20 21v-2a4 4 0 0 0-4-4H8a4 4 0 0 0-4 4v2"></path>
                    </svg>
                </button>
                <div class="dropdown-menu" v-show="menuOpen">
                    <div class="dropdown-user" v-if="userName">
                        <span class="user-name">{{ userName }}</span>
                        <span class="user-role">{{ roleLabel }}</span>
                    </div>
                    <div class="dropdown-divider"></div>
                    <div class="lang-row">
                        <span
                            :class="['lang-btn', { active: currentLang === 'ru' }]"
                            @click="switchLang('ru')"
                        >RU</span>
                        <span
                            :class="['lang-btn', { active: currentLang === 'en' }]"
                            @click="switchLang('en')"
                        >EN</span>
                    </div>
                    <div class="dropdown-divider"></div>
                    <button class="logout-btn" @click="handleLogout">
                        <svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                            <path d="M9 21H5a2 2 0 0 1-2-2V5a2 2 0 0 1 2-2h4"/>
                            <polyline points="16 17 21 12 16 7"/>
                            <line x1="21" y1="12" x2="9" y2="12"/>
                        </svg>
                        {{ $t('header.logout') }}
                    </button>
                </div>
            </div>
        </div>
    </header>
</template>

<script setup>
import { ref, computed, onMounted, onBeforeUnmount } from 'vue'
import { useI18n } from 'vue-i18n'
import { useRouter } from 'vue-router'
import { useAuthStore } from '@/stores/auth'

const { locale, t } = useI18n()
const router = useRouter()
const authStore = useAuthStore()

const menuOpen = ref(false)
const menuRef = ref(null)
const notificationCount = ref(0)

const currentLang = computed(() => locale.value)
const role = computed(() => authStore.userRole)
const userName = computed(() => {
    const u = authStore.user
    if (!u) return ''
    return [u.last_name, u.first_name].filter(Boolean).join(' ')
})

const roleLabel = computed(() => {
    const labels = {
        participant: t('moderator.queue.roles.participant'),
        organizer: t('moderator.queue.roles.organizer'),
        moderator: t('moderator.queue.roles.moderator'),
    }
    return labels[role.value] || role.value
})

function toggleMenu() {
    menuOpen.value = !menuOpen.value
}

function switchLang(lang) {
    locale.value = lang
    localStorage.setItem('app_language', lang)
    menuOpen.value = false
}

function handleLogout() {
    authStore.logout()
    router.push('/login')
}

function handleClickOutside(e) {
    if (menuRef.value && !menuRef.value.contains(e.target)) {
        menuOpen.value = false
    }
}

onMounted(() => document.addEventListener('click', handleClickOutside))
onBeforeUnmount(() => document.removeEventListener('click', handleClickOutside))
</script>

<style scoped lang="scss">
.app-header {
    display: flex;
    align-items: center;
    justify-content: space-between;
    padding: 14px 40px;
    background-color: variables.$white;
    border-bottom: 1px solid variables.$ultra-light-gray;
    position: sticky;
    top: 0;
    z-index: 100;
}

.logo {
    font-size: 22px;
    font-weight: 800;
    color: variables.$main-color;
    text-transform: uppercase;
    letter-spacing: 1px;
}

.nav-links {
    display: flex;
    gap: 32px;
    margin-left: auto;
    margin-right: 40px;

    .nav-link {
        text-decoration: none;
        color: variables.$light-gray;
        font-size: 14px;
        font-weight: 500;
        transition: color 0.2s;

        &:hover,
        &.router-link-active {
            color: variables.$main-color;
        }
    }
}

.header-actions {
    display: flex;
    align-items: center;
    gap: 12px;

    button {
        background: none;
        border: none;
        cursor: pointer;
        position: relative;
        display: flex;
        align-items: center;
    }

    .notification-btn {
        color: variables.$light-gray;
        padding: 6px;
        border-radius: functions.radius('s');
        transition: color 0.2s, background 0.2s;

        &:hover {
            color: variables.$main-color;
            background: variables.$dark-white;
        }

        .badge {
            position: absolute;
            top: 0px;
            right: 0px;
            background: variables.$dangerous;
            color: variables.$white;
            font-size: 10px;
            min-width: 16px;
            height: 16px;
            padding: 0 3px;
            border-radius: 10px;
            font-weight: 700;
            display: flex;
            align-items: center;
            justify-content: center;
        }
    }

    .profile-btn {
        background: variables.$dark-white;
        border-radius: 50%;
        padding: 8px;
        transition: background 0.2s;

        &:hover {
            background: variables.$ultra-light-main-color;
        }
    }
}

.profile-menu {
    position: relative;
}

.dropdown-menu {
    position: absolute;
    top: calc(100% + 8px);
    right: 0;
    background: variables.$white;
    border: 1px solid variables.$ultra-light-gray;
    border-radius: functions.radius('m');
    box-shadow: 0 8px 24px rgba(0, 0, 0, 0.1);
    min-width: 200px;
    z-index: 200;
    overflow: hidden;
}

.dropdown-user {
    padding: 12px 16px;
    display: flex;
    flex-direction: column;
    gap: 2px;

    .user-name {
        font-size: 14px;
        font-weight: 600;
        color: variables.$black;
    }

    .user-role {
        font-size: 12px;
        color: variables.$gray;
    }
}

.dropdown-divider {
    height: 1px;
    background: variables.$ultra-light-gray;
}

.lang-row {
    display: flex;
    gap: 8px;
    padding: 10px 16px;
}

.lang-btn {
    padding: 4px 10px;
    font-size: 13px;
    color: variables.$gray;
    border-radius: functions.radius('xs');
    cursor: pointer;
    transition: all 0.2s;
    user-select: none;

    &.active {
        background: variables.$dark-white;
        color: variables.$black;
        font-weight: 600;
    }

    &:hover:not(.active) {
        color: variables.$black;
    }
}

.logout-btn {
    width: 100%;
    display: flex;
    align-items: center;
    gap: 8px;
    padding: 12px 16px;
    background: none;
    border: none;
    cursor: pointer;
    font-size: 14px;
    color: variables.$dangerous;
    transition: background 0.2s;
    text-align: left;

    &:hover {
        background: #fff5f5;
    }
}
</style>