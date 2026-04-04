import { createRouter, createWebHistory } from 'vue-router'
import { useAuthStore } from '../stores/auth'

const routes = [
    { path: '/', component: () => import('../views/Home.vue') },
    { path: '/login', component: () => import('../views/Login.vue') },
    { path: '/register', component: () => import('../views/Register.vue') },

    {
        path: '/participant/dashboard',
        component: () => import('../views/participant/Dashboard.vue'),
        meta: { requiresAuth: true, roles: ['participant'] },
    },
    {
        path: '/organizer/events',
        component: () => import('../views/organizer/EventList.vue'),
        meta: { requiresAuth: true, roles: ['organizer'] },
    },
    {
        path: '/moderator/queue',
        component: () => import('../views/moderator/ModerationQueue.vue'),
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
    } else if (requiresAuth && allowedRoles && !allowedRoles.includes(authStore.userRole)) {
        next({ name: 'home' });
        return;
    } else {
        next();
    }
});

export default router