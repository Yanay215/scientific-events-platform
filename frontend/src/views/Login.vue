<template>
    <div class="login-container">
        <el-card class="login-card">
            <h2>Вход в систему</h2>
            <el-form @submit.prevent="handleLogin" label-position="top">
                <el-form-item label="Email">
                    <el-input v-model="email" type="email" placeholder="example@example.com" required />
                </el-form-item>
                <el-form-item label="Пароль">
                    <el-input v-model="password" type="password" placeholder="Пароль" show-password required />
                </el-form-item>
                <el-button type="primary" native-type="submit" :loading="loading" class="w-100">Войти</el-button>
                <p v-if="errorMessage" class="error-text"> {{ errorMessage }} </p>
            </el-form>
        </el-card>
    </div>
</template>

<script setup>
import { ref } from 'vue'
import { useRouter } from 'vue-router'
import { useAuthStore } from '@/stores/auth'

const email = ref('');
const password = ref('');
const loading = ref(false);
const errorMessage = ref('');

const router = useRouter();
const authStore = useAuthStore();

const handleLogin = async () => {
    loading.value = true;
    errorMessage.value = '';
    try {
        await authStore.login(email.value, password.value);
        if (authStore.userRole === "participant") router.push('/participant/dashboard');
        else if (authStore.userRole === "organizer") router.push('/organizer/events');
        else if (authStore.userRole === "moderator") router.push('/moderator/queue');
    } catch (error) {
        errorMessage.value = error.message;
    } finally {
        loading.value = false;
    }
}
</script>

<style scoped>
.login-container {
    display: flex;
    justify-content: center;
    align-items: center;
    height: 100vh;
    background-color: #f5f7fa;
}
.login-card {
    width: 400px;
}
.w-100 {
    width: 100%;
}
.error-text {
    color: red;
    margin-top: 10px;
    text-align: center;
}
</style>