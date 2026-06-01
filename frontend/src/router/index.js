import { createRouter, createWebHistory } from 'vue-router'
import { useAuthStore } from '@/stores/auth'

const roleHome = {
    participant: '/participant/dashboard',
    organizer: '/organizer/events',
    moderator: '/moderator/queue',
}

const routes = [
    { path: '/', redirect: '/login' },
    { path: '/login', name: 'login', component: () => import('@/views/auth/Login.vue') },
    { path: '/register', component: () => import('@/views/auth/Registration.vue') },
    { path: '/forgot-password', component: () => import('@/views/auth/PasswordRecovery.vue') },
    { path: '/verify-email', name: 'VerifyEmail', component: () => import('@/views/auth/MailConfirmation.vue'), props: { mode: 'registration' } },
    { path: '/reset-password', name: 'ResetPassword', component: () => import('@/views/auth/MailConfirmation.vue'), props: { mode: 'reset-password' } },
    { path: '/final-registration', component: () => import('@/views/auth/FinalRegistration.vue') },
    { path: '/new-password', component: () => import('@/views/auth/NewPassword.vue') },

    {
        path: '/participant/dashboard',
        component: () => import('@/views/participant/Dashboard.vue'),
        name: 'Dashboard',
        meta: { requiresAuth: true, roles: ['participant'] },
    },
    {
        path: '/participant/registration',
        component: () => import('@/views/participant/RegistrateOnEvent.vue'),
        meta: { requiresAuth: true, roles: ['participant'] },
    },
    {
        path: '/organizer/events',
        component: () => import('@/views/organizer/EventList.vue'),
        name: 'OrganizerDashboard',
        meta: { requiresAuth: true, roles: ['organizer'] },
    },
    {
        path: '/organizer/create-event',
        component: () => import('@/views/organizer/CreateEvent.vue'),
        meta: { requiresAuth: true, roles: ['organizer'] },
    },
    {
        path: '/moderator/queue',
        component: () => import('@/views/moderator/ModerationQueue.vue'),
        name: 'ModeratorDashboard',
        meta: { requiresAuth: true, roles: ['moderator'] },
    },
]

const router = createRouter({
    history: createWebHistory(),
    routes,
})

router.beforeEach(async (to, from, next) => {
    const authStore = useAuthStore();
    const requiresAuth = to.meta.requiresAuth;
    const allowedRoles = to.meta.roles;

    if (requiresAuth && !authStore.isAuthenticated) {
        next({ name: 'login' });
        return;
    }
    if (requiresAuth && allowedRoles && !allowedRoles.includes(authStore.userRole)) {
        const home = roleHome[authStore.userRole];
        next(home || { name: 'login' });
        return;
    }
    next();
});

export default router