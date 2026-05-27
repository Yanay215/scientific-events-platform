import { createRouter, createWebHistory } from 'vue-router'
import { useAuthStore } from '../stores/auth'

const routes = [
    { path: '/login', name: 'login', component: () => import('../views/auth/Login.vue') },
    { path: '/register', component: () => import('../views/auth/Registration.vue') },
    { path: '/forgot-password', component: () => import('../views/auth/PasswordRecovery.vue') },
    { path: '/verify-email', name: 'VerifyEmail', component: () => import('../views/auth/MailConfirmation.vue'), props: { mode: 'registration' } },
    { path: '/reset-password', name: 'ResetPassword', component: () => import('../views/auth/MailConfirmation.vue'), props: { mode: 'reset-password' } },
    { path: '/final-registration', component: () => import('../views/auth/FinalRegistration.vue') },
    { path: '/new-password', component: () => import('../views/auth/NewPassword.vue') },
    // { path: '/', component: () => import('../views/Home.vue') },

    {
        path: '/participant/dashboard',
        component: () => import('../views/participant/Dashboard.vue'),
//        meta: { requiresAuth: true, roles: ['participant'] },
    },
    {
        path: '/participant/registration',
        component: () => import('../views/participant/RegistrateOnEvent.vue'),
    },
    {
        path: '/organizer/events',
        component: () => import('../views/organizer/EventList.vue'),
//        meta: { requiresAuth: true, roles: ['organizer'] },
    },
    { path: '/organizer/create-event', component: () => import('../views/organizer/CreateEvent.vue') },
    {
        path: '/moderator/queue',
        component: () => import('../views/moderator/ModerationQueue.vue'),
//        meta: { requiresAuth: true, roles: ['moderator'] },
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
    } else if (requiresAuth && allowedRoles && !allowedRoles.includes(authStore.userRole)) {
        next({ name: 'home' });
        return;
    } else {
        next();
    }
});

export default router