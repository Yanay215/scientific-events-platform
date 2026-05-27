import { defineStore } from 'pinia'
import axios from 'axios'
import { ElMessage } from 'element-plus'

const apiUrl = import.meta.env.VITE_API_URL || 'http://localhost:8000';
axios.defaults.baseURL = apiUrl;

axios.interceptors.response.use((config) => {
    const token = localStorage.getItem('token');
    if (token) {
        config.headers.Authorization = `Bearer ${token}`;
    }
    return config;
});

export const useAuthStore = defineStore('auth', {
    state: () => ({
        token: localStorage.getItem('token') || null,
        user: JSON.parse(localStorage.getItem('user')) || null,
        currentUserId: localStorage.getItem('user_id') || null,
    }),
    getters: {
        isAuthenticated: (state) => !!state.token,
        userRole: (state) => state.user?.role || null,
    },
    actions: {
        async login(email, password) {
            try {
                const response = await axios.post('/api/auth/login', { email, password });
                this.token = response.data.token;
                this.user = response.data.user;
                this.currentUserId = response.data.user_id;
                localStorage.setItem('token', this.token);
                localStorage.setItem('user', JSON.stringify(this.user));
                localStorage.setItem('user_id', this.currentUserId);
                ElMessage.success('Добро пожаловать, ' + this.user.first_name + '!');
            } catch (error) {
                const errorMessage = error.response?.data?.error || 'Неверный логин или пароль';
                ElMessage.error(errorMessage);
                throw error;
            }
        },
        async register(email, password, firstName, lastName) {
            try {
                const response = await axios.post('/api/auth/register', { email, password, first_name: firstName, last_name: lastName });
                this.token = response.data.token;
                this.user = response.data.user;
                this.currentUserId = response.data.user_id;
                localStorage.setItem('token', this.token);
                localStorage.setItem('user', JSON.stringify(this.user));
                localStorage.setItem('user_id', this.currentUserId);
                ElMessage.success('Регистрация успешна! Код подтверждения отправлен на почту.');
                return response.data;
            } catch (error) {
                const errorMsg = error.response?.data?.error || 'Ошибка при регистрации';
                ElMessage.error(errorMsg);
                throw error;
            }
        },
        async verifyEmail(code) {
            try {
                if (!this.currentUserId) {
                    throw new Error('Необходимо авторизоваться перед продолжением');
                }
                const response = await axios.post('/api/auth/verify', { user_id: this.currentUserId, code: code });
                ElMessage.success('Почта успешно подтверждена!');
                return response.data;
            } catch (error) {
                const errorMsg = error.response?.data?.error || 'Неверный или истекший код подтверждения';
                ElMessage.error(errorMsg);
                throw error;
            }
        },
        logout() {
            this.token = null;
            this.user = null;
            this.currentUserId = null;
            localStorage.removeItem('token');
            localStorage.removeItem('user');
            localStorage.removeItem('user_id');
        },
    },
});