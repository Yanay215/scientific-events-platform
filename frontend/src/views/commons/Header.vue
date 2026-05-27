<template>
    <header class="app-header">
        <div class="logo">ЛОГО</div>
        <nav class="nav-links">
            <RouterLink to="/events" class="nav-link">Мероприятия</RouterLink>
            <RouterLink to="/users" class="nav-link">Пользователи</RouterLink>
        </nav>
        <div class="header-actions">
            <button class="notification-btn">
                <svg width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                    <path d="M18 8A6 6 0 0 0 6 8c0 7-3 9-3 9h18s-3-2-3-9"></path>
                    <path d="M13.73 21a2 2 0 0 1-3.46 0"></path>
                </svg>
                <span class="badge">10</span>
            </button>
            <button class="profile-btn">
                <svg width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="#a8a8a8" stroke-width="2">
                    <circle cx="12" cy="7" r="4"></circle>
                    <path d="M20 21v-2a4 4 0 0 0-4-4H8a4 4 0 0 0-4 4v2"></path>
                </svg>
            </button>
        </div>
    </header>
</template>
<style scoped lang="scss">
.app-header {
    display: flex;
    align-items: center;
    justify-content: space-between;
    padding: 16px 40px;
    background-color: variables.$white;
    border-bottom: 1px solid variables.$ultra-light-gray;
}

.logo {
    font-size: 24px;
    font-weight: 700;
    color: variables.$main-color;
    text-transform: uppercase;
}

.nav-links {
    display: flex;
    gap: 32px;
    margin-left: auto;
    margin-right: 48px;

    .nav-link {
        text-decoration: none;
        color: variables.$light-gray;
        font-size: 14px;
        transition: color 0.2s;

        &:hover, &.router-link-active {
            color: variables.$main-color;
        }
    }
}

.header-actions {
    display: flex;
    align-items: center;
    gap: 16px;

    button {
        background: none;
        border: none;
        cursor: pointer;
        position: relative;
    }

    .notification-btn {
        color: variables.$light-gray;
        
        .badge {
            position: absolute;
            top: -4px;
            right: -6px;
            background: variables.$dangerous;
            color: variables.$white;
            font-size: 10px;
            padding: 2px 4px;
            border-radius: 10px;
            font-weight: bold;
        }
    }

    .profile-btn {
        background: variables.$dark-white;
        border-radius: 50%;
        padding: 8px;
    }
}
</style>