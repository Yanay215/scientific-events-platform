import { defineStore } from 'pinia'
import axios from 'axios'
import { ElMessage } from 'element-plus'

const apiUrl = import.meta.env.VITE_API_URL || 'http://localhost:5173';
axios.defaults.baseURL = apiUrl;

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
        async register(email, password) {
            try {
                const response = await axios.post('/api/auth/register', { email, password });
                if (response.data.verification_id) {
                    localStorage.setItem('verification_id', response.data.verification_id);
                }
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
                const verificationId = localStorage.getItem('verification_id');
                if (!verificationId) {
                    throw new Error('Сессия верификации не найдена. Пожалуйста, зарегистрируйтесь заново.');
                }
                const response = await axios.post('/api/auth/verify', { verification_id: verificationId, code });
                ElMessage.success('Почта успешно подтверждена!');
                return response.data;
            } catch (error) {
                const errorMsg = error.response?.data?.error || 'Неверный или истекший код подтверждения';
                ElMessage.error(errorMsg);
                throw error;
            }
        },
        async completeRegistration(profileData) {
            try {
                const verificationId = localStorage.getItem('verification_id');
                if (!verificationId) {
                    throw new Error('Сессия верификации не найдена. Пожалуйста, зарегистрируйтесь заново.');
                }
                const response = await axios.post('/api/auth/complete', { verification_id: verificationId, ...profileData });
                this.token = response.data.token;
                this.user = response.data.user;
                this.currentUserId = response.data.user?.id;
                localStorage.setItem('token', this.token);
                localStorage.setItem('user', JSON.stringify(this.user));
                localStorage.setItem('user_id', this.currentUserId);
                localStorage.removeItem('verification_id');
                ElMessage.success('Регистрация успешна! Добро пожаловать, ' + this.user.first_name + '!');
                return response.data;
            } catch (error) {
                const errorMsg = error.response?.data?.error || 'Ошибка при завершении регистрации';
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