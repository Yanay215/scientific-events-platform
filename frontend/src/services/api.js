import axios from 'axios';

const apiUrl = import.meta.env.VITE_API_URL || 'http://localhost:5173';
axios.defaults.baseURL = apiUrl;

axios.interceptors.request.use((config) => {
    const token = localStorage.getItem('token');
    if (token) {
        config.headers.Authorization = `Bearer ${token}`;
    }
    return config;
    }, (error) => {
        return Promise.reject(error);
});

export async function getAllUniversities(query) {
    try {
        const response = await axios.get('/api/universities', { 
            params: { q: query }
        });
        return response.data;
    } catch (error) {
        console.error(error);
        throw error;
    }
}