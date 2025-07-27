
// #!/usr/bin/env node

const { spawn } = require('child_process');
const chalk = require('chalk');
const path = require('path');

console.log(chalk.blue.bold('🚀 啟動 IoT Cross-Media 教學系統...\n'));

// 啟動後端
console.log(chalk.yellow('📡 啟動後端服務...'));
const backend = spawn('npm', ['start'], {
    cwd: path.join(__dirname, 'backend-simple'),
    stdio: 'inherit'
});

// 等待 2 秒後啟動前端
setTimeout(() => {
    console.log(chalk.yellow('🌐 啟動前端應用...'));
    const frontend = spawn('npm', ['start'], {
        cwd: path.join(__dirname, 'frontend-simple'),
        stdio: 'inherit'
    });
    
    frontend.on('error', (err) => {
        console.error(chalk.red('前端啟動失敗:'), err);
    });
}, 2000);

backend.on('error', (err) => {
    console.error(chalk.red('後端啟動失敗:'), err);
});

// 處理退出
process.on('SIGINT', () => {
    console.log(chalk.yellow('\n正在關閉系統...'));
    backend.kill();
    process.exit();
});

console.log(chalk.green('\n✅ 系統啟動中，請稍候...'));
console.log(chalk.cyan('📖 前端將在 http://localhost:3000 開啟'));
console.log(chalk.cyan('⚡ 後端 API 運行在 http://localhost:3001'));
console.log(chalk.gray('\n按 Ctrl+C 停止系統'));
